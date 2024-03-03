# PAC (Payson's Anti Cheat)

This is a kernelmode anti cheat that I made with some friends while I was grounded. This is a little bit different
than what I normally do but I think this is great to learn advanced kernel and the other side of cheating.

# Features

- Unlinked process detection
- System module device object verification
- System module .text integrity checks
- Removed thread PspCidTable entry detection
- Process module .text section integrity checks
- Process handle table enumeration
- NMI stackwalking via isr iretq
- Malicious PCI device detection via configuration space scanning
- Hypervisor detection
- Handle stripping via obj callbacks
- HalDispatch and HalPrivateDispatch routine validation
- Extraction of hardware identifiers
- EPT hook detection
- Dynamic import resolving & encryption
- Driver integrity checks both locally and over server
- Dispatch routine validation
- DPC stackwalking via RtlCaptureStackBackTrace
- Attached thread detection
- APC stackwalking via RtlCaptureStackBackTrace

# Planning to add

- ntoskrnl integrity checks
- cr3 protection 
- string, packet and other encryption
- tpm spoofer detection
- pcileech firmware detection 
- testing program to test the features
- simple user mode logger + usermode logging overhaul
- data ptr detction (+ chained data ptr walking)

# known issues

- [Known Issues](https://github.com/paysonism/PAC/issues)

## How to Load Driver

You can load the driver by enabling test signing, mapping with kdmapper, or signing it and using sc start.

# Credits

Based off Donna AC 

Made by [Payson](https://github.com/paysonism)