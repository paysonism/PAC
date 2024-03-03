#include "apc.h"

#include "driver.h"
#include "imports.h"

VOID
GetApcContextByIndex(_Out_ PVOID* Context, _In_ INT Index)
{
        AcquireDriverConfigLock();
        *Context = GetApcContextArray()[Index];
        ReleaseDriverConfigLock();
}

VOID
GetApcContext(_Out_ PVOID* Context, _In_ LONG ContextIdentifier)
{
        AcquireDriverConfigLock();

        for (INT index = 0; index < MAXIMUM_APC_CONTEXTS; index++)
        {
                PAPC_CONTEXT_HEADER header = GetApcContextArray()[index];

                if (!header)
                        continue;

                if (header->context_id != ContextIdentifier)
                        continue;

                *Context = header;
                goto unlock;
        }
unlock:
        ReleaseDriverConfigLock();
}

BOOLEAN
FreeApcContextStructure(_Out_ PAPC_CONTEXT_HEADER Context)
{
        DEBUG_VERBOSE("All APCs executed, freeing context structure");

        for (INT index = 0; index < MAXIMUM_APC_CONTEXTS; index++)
        {
                PUINT64 entry = GetApcContextArray();

                if (entry[index] != Context)
                        continue;

                if (Context->count > 0)
                        return FALSE;

                ImpExFreePoolWithTag(Context, POOL_TAG_APC);
                entry[index] = NULL;
                return TRUE;
        }

        return FALSE;
}

VOID
IncrementApcCount(_In_ LONG ContextId)
{
        PAPC_CONTEXT_HEADER header = NULL;
        GetApcContext(&header, ContextId);

        if (!header)
                return;

        AcquireDriverConfigLock();
        header->count += 1;
        ReleaseDriverConfigLock();
}

VOID
FreeApcAndDecrementApcCount(_Inout_ PRKAPC Apc, _In_ LONG ContextId)
{
        PAPC_CONTEXT_HEADER context = NULL;

        ImpExFreePoolWithTag(Apc, POOL_TAG_APC);
        GetApcContext(&context, ContextId);

        if (!context)
                return;

        AcquireDriverConfigLock();
        context->count -= 1;
        ReleaseDriverConfigLock();
}

NTSTATUS
QueryActiveApcContextsForCompletion()
{
        for (INT index = 0; index < MAXIMUM_APC_CONTEXTS; index++)
        {
                PAPC_CONTEXT_HEADER entry = NULL;
                GetApcContextByIndex(&entry, index);
                AcquireDriverConfigLock();

                if (!entry)
                        goto increment;

                if (entry->count > 0 || entry->allocation_in_progress == TRUE)
                        goto increment;

                switch (entry->context_id)
                {
                case APC_CONTEXT_ID_STACKWALK:
                        FreeApcStackwalkApcContextInformation(entry);
                        FreeApcContextStructure(entry);
                        break;
                }

        increment:
                ReleaseDriverConfigLock();
        }
        return STATUS_SUCCESS;
}

VOID
InsertApcContext(_In_ PVOID Context)
{
        if (IsDriverUnloading())
                return STATUS_UNSUCCESSFUL;

        AcquireDriverConfigLock();
        PAPC_CONTEXT_HEADER header = Context;

        for (INT index = 0; index < MAXIMUM_APC_CONTEXTS; index++)
        {
                PUINT64 entry = GetApcContextArray();

                if (entry[index] == NULL)
                {
                        entry[index] = Context;
                        goto end;
                }
        }
end:
        ReleaseDriverConfigLock();
}

BOOLEAN
DrvUnloadFreeAllApcContextStructures()
{
        AcquireDriverConfigLock();

        for (INT index = 0; index < MAXIMUM_APC_CONTEXTS; index++)
        {
                PUINT64 entry = GetApcContextArray();

                if (entry[index] == NULL)
                        continue;

                PAPC_CONTEXT_HEADER context = entry[index];

                if (context->count > 0)
                {
                        ReleaseDriverConfigLock();
                        return FALSE;
                }

                ImpExFreePoolWithTag(entry, POOL_TAG_APC);
        }
unlock:
        ReleaseDriverConfigLock();
        return TRUE;
}