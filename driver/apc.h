#ifndef APC_H
#define APC_H

#include "common.h"

#include "apc.h"

#include "driver.h"
#include "imports.h"

VOID
GetApcContextByIndex(_Out_ PVOID* Context, _In_ INT Index);

VOID
GetApcContext(_Out_ PVOID* Context, _In_ LONG ContextIdentifier);

BOOLEAN
FreeApcContextStructure(_Out_ PAPC_CONTEXT_HEADER Context);

VOID
IncrementApcCount(_In_ LONG ContextId);

VOID
FreeApcAndDecrementApcCount(_Inout_ PRKAPC Apc, _In_ LONG ContextId);

NTSTATUS
QueryActiveApcContextsForCompletion();

VOID
InsertApcContext(_In_ PVOID Context);

BOOLEAN
DrvUnloadFreeAllApcContextStructures();

#endif