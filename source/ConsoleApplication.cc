// ConsoleApplication.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// https://github.com/yihleego/handle-tools

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include "getopt.h"
#include <string>

#include "ntstatus.h"


#define NT_SUCCESS(Status)    (((NTSTATUS)(Status)) >= 0)

bool endsWith(const std::string& str, const std::string suffix) {
	if (suffix.length() > str.length()) { return false; }
	return (str.rfind(suffix) == (str.length() - suffix.length()));
}

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _SYSTEM_HANDLE {
	PVOID Object;
	HANDLE UniqueProcessId;
	HANDLE HandleValue;
	ULONG GrantedAccess;
	USHORT CreatorBackTraceIndex;
	USHORT ObjectTypeIndex;
	ULONG HandleAttributes;
	ULONG Reserved;
} SYSTEM_HANDLE, * PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX {
	ULONG_PTR HandleCount;
	ULONG_PTR Reserved;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

typedef struct _OBJECT_BASIC_INFORMATION {
	ULONG Attributes;
	ACCESS_MASK GrantedAccess;
	ULONG HandleCount;
	ULONG PointerCount;
	ULONG PagedPoolCharge;
	ULONG NonPagedPoolCharge;
	ULONG Reserved[3];
	ULONG NameInfoSize;
	ULONG TypeInfoSize;
	ULONG SecurityDescriptorSize;
	LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION, * POBJECT_BASIC_INFORMATION;

typedef struct _OBJECT_NAME_INFORMATION {
	UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

typedef struct _OBJECT_TYPE_INFORMATION {
	UNICODE_STRING TypeName;
	ULONG Reserved[22];    // reserved for internal use
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation, // q: SYSTEM_BASIC_INFORMATION
	SystemProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
	SystemPerformanceInformation, // q: SYSTEM_PERFORMANCE_INFORMATION
	SystemTimeOfDayInformation, // q: SYSTEM_TIMEOFDAY_INFORMATION
	SystemPathInformation, // not implemented
	SystemProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
	SystemCallCountInformation, // q: SYSTEM_CALL_COUNT_INFORMATION
	SystemDeviceInformation, // q: SYSTEM_DEVICE_INFORMATION
	SystemProcessorPerformanceInformation, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
	SystemFlagsInformation, // q: SYSTEM_FLAGS_INFORMATION
	SystemCallTimeInformation, // not implemented // SYSTEM_CALL_TIME_INFORMATION // 10
	SystemModuleInformation, // q: RTL_PROCESS_MODULES
	SystemLocksInformation, // q: RTL_PROCESS_LOCKS
	SystemStackTraceInformation, // q: RTL_PROCESS_BACKTRACES
	SystemPagedPoolInformation, // not implemented
	SystemNonPagedPoolInformation, // not implemented
	SystemHandleInformation, // q: SYSTEM_HANDLE_INFORMATION
	SystemObjectInformation, // q: SYSTEM_OBJECTTYPE_INFORMATION mixed with SYSTEM_OBJECT_INFORMATION
	SystemPageFileInformation, // q: SYSTEM_PAGEFILE_INFORMATION
	SystemVdmInstemulInformation, // q: SYSTEM_VDM_INSTEMUL_INFO
	SystemVdmBopInformation, // not implemented // 20
	SystemFileCacheInformation, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemCache)
	SystemPoolTagInformation, // q: SYSTEM_POOLTAG_INFORMATION
	SystemInterruptInformation, // q: SYSTEM_INTERRUPT_INFORMATION
	SystemDpcBehaviorInformation, // q: SYSTEM_DPC_BEHAVIOR_INFORMATION; s: SYSTEM_DPC_BEHAVIOR_INFORMATION (requires SeLoadDriverPrivilege)
	SystemFullMemoryInformation, // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
	SystemLoadGdiDriverInformation, // s (kernel-mode only)
	SystemUnloadGdiDriverInformation, // s (kernel-mode only)
	SystemTimeAdjustmentInformation, // q: SYSTEM_QUERY_TIME_ADJUST_INFORMATION; s: SYSTEM_SET_TIME_ADJUST_INFORMATION (requires SeSystemtimePrivilege)
	SystemSummaryMemoryInformation, // not implemented // SYSTEM_MEMORY_USAGE_INFORMATION
	SystemMirrorMemoryInformation, // s (requires license value "Kernel-MemoryMirroringSupported") (requires SeShutdownPrivilege) // 30
	SystemPerformanceTraceInformation, // q; s: (type depends on EVENT_TRACE_INFORMATION_CLASS)
	SystemObsolete0, // not implemented
	SystemExceptionInformation, // q: SYSTEM_EXCEPTION_INFORMATION
	SystemCrashDumpStateInformation, // s: SYSTEM_CRASH_DUMP_STATE_INFORMATION (requires SeDebugPrivilege)
	SystemKernelDebuggerInformation, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION
	SystemContextSwitchInformation, // q: SYSTEM_CONTEXT_SWITCH_INFORMATION
	SystemRegistryQuotaInformation, // q: SYSTEM_REGISTRY_QUOTA_INFORMATION; s (requires SeIncreaseQuotaPrivilege)
	SystemExtendServiceTableInformation, // s (requires SeLoadDriverPrivilege) // loads win32k only
	SystemPrioritySeperation, // s (requires SeTcbPrivilege)
	SystemVerifierAddDriverInformation, // s (requires SeDebugPrivilege) // 40
	SystemVerifierRemoveDriverInformation, // s (requires SeDebugPrivilege)
	SystemProcessorIdleInformation, // q: SYSTEM_PROCESSOR_IDLE_INFORMATION
	SystemLegacyDriverInformation, // q: SYSTEM_LEGACY_DRIVER_INFORMATION
	SystemCurrentTimeZoneInformation, // q; s: RTL_TIME_ZONE_INFORMATION
	SystemLookasideInformation, // q: SYSTEM_LOOKASIDE_INFORMATION
	SystemTimeSlipNotification, // s: HANDLE (NtCreateEvent) (requires SeSystemtimePrivilege)
	SystemSessionCreate, // not implemented
	SystemSessionDetach, // not implemented
	SystemSessionInformation, // not implemented (SYSTEM_SESSION_INFORMATION)
	SystemRangeStartInformation, // q: SYSTEM_RANGE_START_INFORMATION // 50
	SystemVerifierInformation, // q: SYSTEM_VERIFIER_INFORMATION; s (requires SeDebugPrivilege)
	SystemVerifierThunkExtend, // s (kernel-mode only)
	SystemSessionProcessInformation, // q: SYSTEM_SESSION_PROCESS_INFORMATION
	SystemLoadGdiDriverInSystemSpace, // s: SYSTEM_GDI_DRIVER_INFORMATION (kernel-mode only) (same as SystemLoadGdiDriverInformation)
	SystemNumaProcessorMap, // q: SYSTEM_NUMA_INFORMATION
	SystemPrefetcherInformation, // q; s: PREFETCHER_INFORMATION // PfSnQueryPrefetcherInformation
	SystemExtendedProcessInformation, // q: SYSTEM_PROCESS_INFORMATION
	SystemRecommendedSharedDataAlignment, // q: ULONG // KeGetRecommendedSharedDataAlignment
	SystemComPlusPackage, // q; s: ULONG
	SystemNumaAvailableMemory, // q: SYSTEM_NUMA_INFORMATION // 60
	SystemProcessorPowerInformation, // q: SYSTEM_PROCESSOR_POWER_INFORMATION
	SystemEmulationBasicInformation, // q: SYSTEM_BASIC_INFORMATION
	SystemEmulationProcessorInformation, // q: SYSTEM_PROCESSOR_INFORMATION
	SystemExtendedHandleInformation, // q: SYSTEM_HANDLE_INFORMATION_EX
	SystemLostDelayedWriteInformation, // q: ULONG
	SystemBigPoolInformation, // q: SYSTEM_BIGPOOL_INFORMATION
	SystemSessionPoolTagInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION
	SystemSessionMappedViewInformation, // q: SYSTEM_SESSION_MAPPED_VIEW_INFORMATION
	SystemHotpatchInformation, // q; s: SYSTEM_HOTPATCH_CODE_INFORMATION
	SystemObjectSecurityMode, // q: ULONG // 70
	SystemWatchdogTimerHandler, // s: SYSTEM_WATCHDOG_HANDLER_INFORMATION // (kernel-mode only)
	SystemWatchdogTimerInformation, // q: SYSTEM_WATCHDOG_TIMER_INFORMATION // (kernel-mode only)
	SystemLogicalProcessorInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION
	SystemWow64SharedInformationObsolete, // not implemented
	SystemRegisterFirmwareTableInformationHandler, // s: SYSTEM_FIRMWARE_TABLE_HANDLER // (kernel-mode only)
	SystemFirmwareTableInformation, // SYSTEM_FIRMWARE_TABLE_INFORMATION
	SystemModuleInformationEx, // q: RTL_PROCESS_MODULE_INFORMATION_EX
	SystemVerifierTriageInformation, // not implemented
	SystemSuperfetchInformation, // q; s: SUPERFETCH_INFORMATION // PfQuerySuperfetchInformation
	SystemMemoryListInformation, // q: SYSTEM_MEMORY_LIST_INFORMATION; s: SYSTEM_MEMORY_LIST_COMMAND (requires SeProfileSingleProcessPrivilege) // 80
	SystemFileCacheInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (same as SystemFileCacheInformation)
	SystemThreadPriorityClientIdInformation, // s: SYSTEM_THREAD_CID_PRIORITY_INFORMATION (requires SeIncreaseBasePriorityPrivilege)
	SystemProcessorIdleCycleTimeInformation, // q: SYSTEM_PROCESSOR_IDLE_CYCLE_TIME_INFORMATION[]
	SystemVerifierCancellationInformation, // SYSTEM_VERIFIER_CANCELLATION_INFORMATION // name:wow64:whNT32QuerySystemVerifierCancellationInformation
	SystemProcessorPowerInformationEx, // not implemented
	SystemRefTraceInformation, // q; s: SYSTEM_REF_TRACE_INFORMATION // ObQueryRefTraceInformation
	SystemSpecialPoolInformation, // q; s: SYSTEM_SPECIAL_POOL_INFORMATION (requires SeDebugPrivilege) // MmSpecialPoolTag, then MmSpecialPoolCatchOverruns != 0
	SystemProcessIdInformation, // q: SYSTEM_PROCESS_ID_INFORMATION
	SystemErrorPortInformation, // s (requires SeTcbPrivilege)
	SystemBootEnvironmentInformation, // q: SYSTEM_BOOT_ENVIRONMENT_INFORMATION // 90
	SystemHypervisorInformation, // q: SYSTEM_HYPERVISOR_QUERY_INFORMATION
	SystemVerifierInformationEx, // q; s: SYSTEM_VERIFIER_INFORMATION_EX
	SystemTimeZoneInformation, // q; s: RTL_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
	SystemImageFileExecutionOptionsInformation, // s: SYSTEM_IMAGE_FILE_EXECUTION_OPTIONS_INFORMATION (requires SeTcbPrivilege)
	SystemCoverageInformation, // q: COVERAGE_MODULES s: COVERAGE_MODULE_REQUEST // ExpCovQueryInformation (requires SeDebugPrivilege)
	SystemPrefetchPatchInformation, // SYSTEM_PREFETCH_PATCH_INFORMATION
	SystemVerifierFaultsInformation, // s: SYSTEM_VERIFIER_FAULTS_INFORMATION (requires SeDebugPrivilege)
	SystemSystemPartitionInformation, // q: SYSTEM_SYSTEM_PARTITION_INFORMATION
	SystemSystemDiskInformation, // q: SYSTEM_SYSTEM_DISK_INFORMATION
	SystemProcessorPerformanceDistribution, // q: SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION // 100
	SystemNumaProximityNodeInformation, // q; s: SYSTEM_NUMA_PROXIMITY_MAP
	SystemDynamicTimeZoneInformation, // q; s: RTL_DYNAMIC_TIME_ZONE_INFORMATION (requires SeTimeZonePrivilege)
	SystemCodeIntegrityInformation, // q: SYSTEM_CODEINTEGRITY_INFORMATION // SeCodeIntegrityQueryInformation
	SystemProcessorMicrocodeUpdateInformation, // s: SYSTEM_PROCESSOR_MICROCODE_UPDATE_INFORMATION
	SystemProcessorBrandString, // q: CHAR[] // HaliQuerySystemInformation -> HalpGetProcessorBrandString, info class 23
	SystemVirtualAddressInformation, // q: SYSTEM_VA_LIST_INFORMATION[]; s: SYSTEM_VA_LIST_INFORMATION[] (requires SeIncreaseQuotaPrivilege) // MmQuerySystemVaInformation
	SystemLogicalProcessorAndGroupInformation, // q: SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX // since WIN7 // KeQueryLogicalProcessorRelationship
	SystemProcessorCycleTimeInformation, // q: SYSTEM_PROCESSOR_CYCLE_TIME_INFORMATION[]
	SystemStoreInformation, // q; s: SYSTEM_STORE_INFORMATION (requires SeProfileSingleProcessPrivilege) // SmQueryStoreInformation
	SystemRegistryAppendString, // s: SYSTEM_REGISTRY_APPEND_STRING_PARAMETERS // 110
	SystemAitSamplingValue, // s: ULONG (requires SeProfileSingleProcessPrivilege)
	SystemVhdBootInformation, // q: SYSTEM_VHD_BOOT_INFORMATION
	SystemCpuQuotaInformation, // q; s: PS_CPU_QUOTA_QUERY_INFORMATION
	SystemNativeBasicInformation, // q: SYSTEM_BASIC_INFORMATION
	SystemErrorPortTimeouts, // SYSTEM_ERROR_PORT_TIMEOUTS
	SystemLowPriorityIoInformation, // q: SYSTEM_LOW_PRIORITY_IO_INFORMATION
	SystemTpmBootEntropyInformation, // q: TPM_BOOT_ENTROPY_NT_RESULT // ExQueryTpmBootEntropyInformation
	SystemVerifierCountersInformation, // q: SYSTEM_VERIFIER_COUNTERS_INFORMATION
	SystemPagedPoolInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypePagedPool)
	SystemSystemPtesInformationEx, // q: SYSTEM_FILECACHE_INFORMATION; s (requires SeIncreaseQuotaPrivilege) (info for WorkingSetTypeSystemPtes) // 120
	SystemNodeDistanceInformation,
	SystemAcpiAuditInformation, // q: SYSTEM_ACPI_AUDIT_INFORMATION // HaliQuerySystemInformation -> HalpAuditQueryResults, info class 26
	SystemBasicPerformanceInformation, // q: SYSTEM_BASIC_PERFORMANCE_INFORMATION // name:wow64:whNtQuerySystemInformation_SystemBasicPerformanceInformation
	SystemQueryPerformanceCounterInformation, // q: SYSTEM_QUERY_PERFORMANCE_COUNTER_INFORMATION // since WIN7 SP1
	SystemSessionBigPoolInformation, // q: SYSTEM_SESSION_POOLTAG_INFORMATION // since WIN8
	SystemBootGraphicsInformation, // q; s: SYSTEM_BOOT_GRAPHICS_INFORMATION (kernel-mode only)
	SystemScrubPhysicalMemoryInformation, // q; s: MEMORY_SCRUB_INFORMATION
	SystemBadPageInformation,
	SystemProcessorProfileControlArea, // q; s: SYSTEM_PROCESSOR_PROFILE_CONTROL_AREA
	SystemCombinePhysicalMemoryInformation, // s: MEMORY_COMBINE_INFORMATION, MEMORY_COMBINE_INFORMATION_EX, MEMORY_COMBINE_INFORMATION_EX2 // 130
	SystemEntropyInterruptTimingInformation, // q; s: SYSTEM_ENTROPY_TIMING_INFORMATION
	SystemConsoleInformation, // q: SYSTEM_CONSOLE_INFORMATION
	SystemPlatformBinaryInformation, // q: SYSTEM_PLATFORM_BINARY_INFORMATION (requires SeTcbPrivilege)
	SystemPolicyInformation, // q: SYSTEM_POLICY_INFORMATION
	SystemHypervisorProcessorCountInformation, // q: SYSTEM_HYPERVISOR_PROCESSOR_COUNT_INFORMATION
	SystemDeviceDataInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
	SystemDeviceDataEnumerationInformation, // q: SYSTEM_DEVICE_DATA_INFORMATION
	SystemMemoryTopologyInformation, // q: SYSTEM_MEMORY_TOPOLOGY_INFORMATION
	SystemMemoryChannelInformation, // q: SYSTEM_MEMORY_CHANNEL_INFORMATION
	SystemBootLogoInformation, // q: SYSTEM_BOOT_LOGO_INFORMATION // 140
	SystemProcessorPerformanceInformationEx, // q: SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION_EX // since WINBLUE
	SystemCriticalProcessErrorLogInformation,
	SystemSecureBootPolicyInformation, // q: SYSTEM_SECUREBOOT_POLICY_INFORMATION
	SystemPageFileInformationEx, // q: SYSTEM_PAGEFILE_INFORMATION_EX
	SystemSecureBootInformation, // q: SYSTEM_SECUREBOOT_INFORMATION
	SystemEntropyInterruptTimingRawInformation,
	SystemPortableWorkspaceEfiLauncherInformation, // q: SYSTEM_PORTABLE_WORKSPACE_EFI_LAUNCHER_INFORMATION
	SystemFullProcessInformation, // q: SYSTEM_PROCESS_INFORMATION with SYSTEM_PROCESS_INFORMATION_EXTENSION (requires admin)
	SystemKernelDebuggerInformationEx, // q: SYSTEM_KERNEL_DEBUGGER_INFORMATION_EX
	SystemBootMetadataInformation, // 150
	SystemSoftRebootInformation, // q: ULONG
	SystemElamCertificateInformation, // s: SYSTEM_ELAM_CERTIFICATE_INFORMATION
	SystemOfflineDumpConfigInformation, // q: OFFLINE_CRASHDUMP_CONFIGURATION_TABLE_V2
	SystemProcessorFeaturesInformation, // q: SYSTEM_PROCESSOR_FEATURES_INFORMATION
	SystemRegistryReconciliationInformation, // s: NULL (requires admin) (flushes registry hives)
	SystemEdidInformation, // q: SYSTEM_EDID_INFORMATION
	SystemManufacturingInformation, // q: SYSTEM_MANUFACTURING_INFORMATION // since THRESHOLD
	SystemEnergyEstimationConfigInformation, // q: SYSTEM_ENERGY_ESTIMATION_CONFIG_INFORMATION
	SystemHypervisorDetailInformation, // q: SYSTEM_HYPERVISOR_DETAIL_INFORMATION
	SystemProcessorCycleStatsInformation, // q: SYSTEM_PROCESSOR_CYCLE_STATS_INFORMATION // 160
	SystemVmGenerationCountInformation,
	SystemTrustedPlatformModuleInformation, // q: SYSTEM_TPM_INFORMATION
	SystemKernelDebuggerFlags, // SYSTEM_KERNEL_DEBUGGER_FLAGS
	SystemCodeIntegrityPolicyInformation, // q: SYSTEM_CODEINTEGRITYPOLICY_INFORMATION
	SystemIsolatedUserModeInformation, // q: SYSTEM_ISOLATED_USER_MODE_INFORMATION
	SystemHardwareSecurityTestInterfaceResultsInformation,
	SystemSingleModuleInformation, // q: SYSTEM_SINGLE_MODULE_INFORMATION
	SystemAllowedCpuSetsInformation,
	SystemVsmProtectionInformation, // q: SYSTEM_VSM_PROTECTION_INFORMATION (previously SystemDmaProtectionInformation)
	SystemInterruptCpuSetsInformation, // q: SYSTEM_INTERRUPT_CPU_SET_INFORMATION // 170
	SystemSecureBootPolicyFullInformation, // q: SYSTEM_SECUREBOOT_POLICY_FULL_INFORMATION
	SystemCodeIntegrityPolicyFullInformation,
	SystemAffinitizedInterruptProcessorInformation, // (requires SeIncreaseBasePriorityPrivilege)
	SystemRootSiloInformation, // q: SYSTEM_ROOT_SILO_INFORMATION
	SystemCpuSetInformation, // q: SYSTEM_CPU_SET_INFORMATION // since THRESHOLD2
	SystemCpuSetTagInformation, // q: SYSTEM_CPU_SET_TAG_INFORMATION
	SystemWin32WerStartCallout,
	SystemSecureKernelProfileInformation, // q: SYSTEM_SECURE_KERNEL_HYPERGUARD_PROFILE_INFORMATION
	SystemCodeIntegrityPlatformManifestInformation, // q: SYSTEM_SECUREBOOT_PLATFORM_MANIFEST_INFORMATION // since REDSTONE
	SystemInterruptSteeringInformation, // SYSTEM_INTERRUPT_STEERING_INFORMATION_INPUT // 180
	SystemSupportedProcessorArchitectures, // in: HANDLE, out: SYSTEM_SUPPORTED_PROCESSOR_ARCHITECTURES_INFORMATION[] (Max 5 structs) // NtQuerySystemInformationEx
	SystemMemoryUsageInformation, // q: SYSTEM_MEMORY_USAGE_INFORMATION
	SystemCodeIntegrityCertificateInformation, // q: SYSTEM_CODEINTEGRITY_CERTIFICATE_INFORMATION
	SystemPhysicalMemoryInformation, // q: SYSTEM_PHYSICAL_MEMORY_INFORMATION // since REDSTONE2
	SystemControlFlowTransition,
	SystemKernelDebuggingAllowed, // s: ULONG
	SystemActivityModerationExeState, // SYSTEM_ACTIVITY_MODERATION_EXE_STATE
	SystemActivityModerationUserSettings, // SYSTEM_ACTIVITY_MODERATION_USER_SETTINGS
	SystemCodeIntegrityPoliciesFullInformation,
	SystemCodeIntegrityUnlockInformation, // SYSTEM_CODEINTEGRITY_UNLOCK_INFORMATION // 190
	SystemIntegrityQuotaInformation,
	SystemFlushInformation, // q: SYSTEM_FLUSH_INFORMATION
	SystemProcessorIdleMaskInformation, // q: ULONG_PTR // since REDSTONE3
	SystemSecureDumpEncryptionInformation,
	SystemWriteConstraintInformation, // SYSTEM_WRITE_CONSTRAINT_INFORMATION
	SystemKernelVaShadowInformation, // SYSTEM_KERNEL_VA_SHADOW_INFORMATION
	SystemHypervisorSharedPageInformation, // SYSTEM_HYPERVISOR_SHARED_PAGE_INFORMATION // since REDSTONE4
	SystemFirmwareBootPerformanceInformation,
	SystemCodeIntegrityVerificationInformation, // SYSTEM_CODEINTEGRITYVERIFICATION_INFORMATION
	SystemFirmwarePartitionInformation, // SYSTEM_FIRMWARE_PARTITION_INFORMATION // 200
	SystemSpeculationControlInformation, // SYSTEM_SPECULATION_CONTROL_INFORMATION // (CVE-2017-5715) REDSTONE3 and above.
	SystemDmaGuardPolicyInformation, // SYSTEM_DMA_GUARD_POLICY_INFORMATION
	SystemEnclaveLaunchControlInformation, // SYSTEM_ENCLAVE_LAUNCH_CONTROL_INFORMATION
	SystemWorkloadAllowedCpuSetsInformation, // SYSTEM_WORKLOAD_ALLOWED_CPU_SET_INFORMATION // since REDSTONE5
	SystemCodeIntegrityUnlockModeInformation,
	SystemLeapSecondInformation, // SYSTEM_LEAP_SECOND_INFORMATION
	SystemFlags2Information, // q: SYSTEM_FLAGS_INFORMATION
	SystemSecurityModelInformation, // SYSTEM_SECURITY_MODEL_INFORMATION // since 19H1
	SystemCodeIntegritySyntheticCacheInformation,
	SystemFeatureConfigurationInformation, // SYSTEM_FEATURE_CONFIGURATION_INFORMATION // since 20H1 // 210
	SystemFeatureConfigurationSectionInformation, // SYSTEM_FEATURE_CONFIGURATION_SECTIONS_INFORMATION
	SystemFeatureUsageSubscriptionInformation, // SYSTEM_FEATURE_USAGE_SUBSCRIPTION_DETAILS
	SystemSecureSpeculationControlInformation, // SECURE_SPECULATION_CONTROL_INFORMATION
	SystemSpacesBootInformation, // since 20H2
	SystemFwRamdiskInformation, // SYSTEM_FIRMWARE_RAMDISK_INFORMATION
	SystemWheaIpmiHardwareInformation,
	SystemDifSetRuleClassInformation,
	SystemDifClearRuleClassInformation,
	SystemDifApplyPluginVerificationOnDriver,
	SystemDifRemovePluginVerificationOnDriver, // 220
	SystemShadowStackInformation, // SYSTEM_SHADOW_STACK_INFORMATION
	SystemBuildVersionInformation, // SYSTEM_BUILD_VERSION_INFORMATION
	SystemPoolLimitInformation, // SYSTEM_POOL_LIMIT_INFORMATION
	SystemCodeIntegrityAddDynamicStore,
	SystemCodeIntegrityClearDynamicStores,
	SystemDifPoolTrackingInformation,
	SystemPoolZeroingInformation, // SYSTEM_POOL_ZEROING_INFORMATION
	MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS;

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation = 0, // q: OBJECT_BASIC_INFORMATION
	ObjectNameInformation = 1, // q: OBJECT_NAME_INFORMATION
	ObjectTypeInformation = 2, // q: OBJECT_TYPE_INFORMATION
	ObjectTypesInformation, // q: OBJECT_TYPES_INFORMATION
	ObjectHandleFlagInformation, // qs: OBJECT_HANDLE_FLAG_INFORMATION
	ObjectSessionInformation, // s: void // change object session // (requires SeTcbPrivilege)
	ObjectSessionObjectInformation, // s: void // change object session // (requires SeTcbPrivilege)
	MaxObjectInfoClass
} OBJECT_INFORMATION_CLASS;

typedef NTSTATUS(WINAPI* PNtQuerySystemInformation)(
	_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_   PVOID SystemInformation,
	_In_      ULONG SystemInformationLength,
	_Out_opt_ PULONG ReturnLength
	);

typedef NTSTATUS(WINAPI* PNtQueryObject)(
	_In_opt_  HANDLE Handle,
	_In_      OBJECT_INFORMATION_CLASS ObjectInformationClass,
	_Out_opt_ PVOID ObjectInformation,
	_In_      ULONG ObjectInformationLength,
	_Out_opt_ PULONG ReturnLength);

typedef NTSTATUS(WINAPI* PNtDuplicateObject)(
	_In_      HANDLE SourceProcessHandle,
	_In_      HANDLE SourceHandle,
	_In_opt_  HANDLE TargetProcessHandle,
	_Out_opt_ PHANDLE TargetHandle,
	_In_      ACCESS_MASK DesiredAccess,
	_In_      ULONG HandleAttributes,
	_In_      ULONG Options
	);

int FindHandles(ULONG pid, LPSTR handleName, BOOL closeHandle, BOOL suffix) {
	HMODULE ntdll = GetModuleHandle(TEXT("ntdll.dll"));
	if (NULL == ntdll) {
		printf("Failed to load 'ntdll.dll'\n");
		return 0;
	}
	PNtQuerySystemInformation pQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(ntdll, "NtQuerySystemInformation");
	PNtQueryObject pQueryObject = (PNtQueryObject)GetProcAddress(ntdll, "NtQueryObject");
	PNtDuplicateObject pDuplicateObject = (PNtDuplicateObject)GetProcAddress(ntdll, "NtDuplicateObject");
	if (NULL == pQuerySystemInformation || NULL == pQueryObject || NULL == pDuplicateObject) {
		printf("Failed to call 'GetProcAddress()'\n");
		return 0;
	}

	ULONG len = 0x10000;
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION_EX pHandleInfo = NULL;
	do {
		if (len > 0x4000000) {
			return 0;
		}
		len *= 2;
		pHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)GlobalAlloc(GMEM_ZEROINIT, len);
		status = pQuerySystemInformation(SystemExtendedHandleInformation, pHandleInfo, len, &len);
	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	if (!NT_SUCCESS(status)) {
		printf("Failed to call 'NtQuerySystemInformation()' with error code 0x%X\n", status);
		return 0;
	}

	HANDLE currentProcess = GetCurrentProcess();
	for (int i = 0; i < pHandleInfo->HandleCount; i++) {
		SYSTEM_HANDLE handle = pHandleInfo->Handles[i];
		PVOID object = handle.Object;
		HANDLE handleValue = handle.HandleValue;
		HANDLE uniqueProcessId = handle.UniqueProcessId;
		if (NULL != pid && HandleToLong(uniqueProcessId) != pid) {
			continue;
		}
		LPSTR pName = NULL;
		LPSTR pType = NULL;
		HANDLE sourceProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_DUP_HANDLE | PROCESS_SUSPEND_RESUME, FALSE, HandleToULong(uniqueProcessId));
		HANDLE targetHandle = NULL;
		NTSTATUS status = pDuplicateObject(sourceProcess, handleValue, currentProcess, &targetHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
		if (NT_SUCCESS(status)) {
			//printf("Failed to call 'NtDuplicateObject()' with error code 0x%X\n", status);
			POBJECT_NAME_INFORMATION pNameInfo = (POBJECT_NAME_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, len);
			POBJECT_TYPE_INFORMATION pTypeInfo = (POBJECT_TYPE_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, len);
			if (NT_SUCCESS(pQueryObject(targetHandle, ObjectNameInformation, pNameInfo, len, NULL))) {
				pName = (LPSTR)GlobalAlloc(GMEM_ZEROINIT, pNameInfo->Name.Length);
				WideCharToMultiByte(CP_ACP, 0, pNameInfo->Name.Buffer, -1, pName, pNameInfo->Name.Length, NULL, NULL);
			}
			if (NT_SUCCESS(pQueryObject(targetHandle, ObjectTypeInformation, pTypeInfo, len, NULL))) {
				pType = (LPSTR)GlobalAlloc(GMEM_ZEROINIT, pTypeInfo->TypeName.Length);
				WideCharToMultiByte(CP_ACP, 0, pTypeInfo->TypeName.Buffer, -1, pType, pTypeInfo->TypeName.Length, NULL, NULL);
			}
		}
		if (NULL != handleName) {
			if (suffix) {
				if (NULL == pName || !endsWith(std::string(pName), std::string(handleName))) {
					continue;
				}
			}
			else {
				if (NULL == pName || 0 != strcmp(pName, handleName)) {
					continue;
				}
			}

			if (TRUE == closeHandle) {
				HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, HandleToLong(uniqueProcessId));
				DuplicateHandle(hProcess, handleValue, 0, 0, 0, 0, DUPLICATE_CLOSE_SOURCE);
				CloseHandle(hProcess);
			}
		}
		printf("PID: %-6d\t", uniqueProcessId);
		printf("Handle: 0x%-3x\t", handleValue);
		printf("Object: 0x%-8X\t", object);
		printf("Type: %-20s\t", NULL != pType ? pType : "<Unknown type>");
		printf("Name: %-30s\t", NULL != pName ? pName : "");
		printf("\n");
	}
	return 1;
}

int DisplayHandles() {
	return FindHandles(NULL, NULL, FALSE, FALSE);
}

int DisplayHandles(ULONG pid) {
	return FindHandles(pid, NULL, FALSE, FALSE);
}

int FindHandle(ULONG pid, LPSTR handleName) {
	return FindHandles(pid, handleName, FALSE, FALSE);
}

int CloseHandle(ULONG pid, LPSTR handleName) {
	return FindHandles(pid, handleName, TRUE, FALSE);
}

HANDLE FindHandleByName(ULONG pid, LPSTR handleName) {
	HMODULE ntdll = GetModuleHandle(TEXT("ntdll.dll"));
	if (NULL == ntdll) {
		printf("Failed to load 'ntdll.dll'\n");
		return 0;
	}
	PNtQuerySystemInformation pQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(ntdll, "NtQuerySystemInformation");
	PNtQueryObject pQueryObject = (PNtQueryObject)GetProcAddress(ntdll, "NtQueryObject");
	PNtDuplicateObject pDuplicateObject = (PNtDuplicateObject)GetProcAddress(ntdll, "NtDuplicateObject");
	if (NULL == pQuerySystemInformation || NULL == pQueryObject || NULL == pDuplicateObject) {
		printf("Failed to call 'GetProcAddress()'\n");
		return 0;
	}

	ULONG len = 0x10000;
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION_EX pHandleInfo = NULL;
	do {
		if (len > 0x4000000) {
			return 0;
		}
		len *= 2;
		pHandleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)GlobalAlloc(GMEM_ZEROINIT, len);
		status = pQuerySystemInformation(SystemExtendedHandleInformation, pHandleInfo, len, &len);
	} while (status == STATUS_INFO_LENGTH_MISMATCH);

	if (!NT_SUCCESS(status)) {
		printf("Failed to call 'NtQuerySystemInformation()' with error code 0x%X\n", status);
		return 0;
	}

	HANDLE currentProcess = GetCurrentProcess();
	for (int i = 0; i < pHandleInfo->HandleCount; i++) {
		SYSTEM_HANDLE handle = pHandleInfo->Handles[i];
		PVOID object = handle.Object;
		HANDLE handleValue = handle.HandleValue;
		HANDLE uniqueProcessId = handle.UniqueProcessId;
		if (NULL != pid && HandleToLong(uniqueProcessId) != pid) {
			continue;
		}
		LPSTR pName = NULL;
		LPSTR pType = NULL;
		HANDLE sourceProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_DUP_HANDLE | PROCESS_SUSPEND_RESUME, FALSE, HandleToULong(uniqueProcessId));
		HANDLE targetHandle = NULL;
		NTSTATUS status = pDuplicateObject(sourceProcess, handleValue, currentProcess, &targetHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
		if (NT_SUCCESS(status)) {
			//printf("Failed to call 'NtDuplicateObject()' with error code 0x%X\n", status);
			POBJECT_NAME_INFORMATION pNameInfo = (POBJECT_NAME_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, len);
			POBJECT_TYPE_INFORMATION pTypeInfo = (POBJECT_TYPE_INFORMATION)GlobalAlloc(GMEM_ZEROINIT, len);
			if (NT_SUCCESS(pQueryObject(targetHandle, ObjectNameInformation, pNameInfo, len, NULL))) {
				pName = (LPSTR)GlobalAlloc(GMEM_ZEROINIT, pNameInfo->Name.Length);
				WideCharToMultiByte(CP_ACP, 0, pNameInfo->Name.Buffer, -1, pName, pNameInfo->Name.Length, NULL, NULL);
			}
			if (NT_SUCCESS(pQueryObject(targetHandle, ObjectTypeInformation, pTypeInfo, len, NULL))) {
				pType = (LPSTR)GlobalAlloc(GMEM_ZEROINIT, pTypeInfo->TypeName.Length);
				WideCharToMultiByte(CP_ACP, 0, pTypeInfo->TypeName.Buffer, -1, pType, pTypeInfo->TypeName.Length, NULL, NULL);
			}
		}
		if (NULL != handleName) {

			if (NULL == pName || 0 != strcmp(pName, handleName)) {
				continue;
			}
			return handleValue;
		}

	}
}


std::wstring Utf8ToUnicode(const char* buffer) {
	int c_size = MultiByteToWideChar(CP_UTF8, 0, buffer, -1, NULL, 0);
	if (c_size > 0) {
		wchar_t* temp = new wchar_t[c_size + 1];
		MultiByteToWideChar(CP_UTF8, 0, buffer, -1, temp, c_size);
		temp[c_size] = L'\0';
		std::wstring ret(temp);
		delete[] temp;
		temp = NULL;
		return ret;
	}
	return std::wstring();
}

DWORD  GetPIDForProcess(wchar_t* process)
{
	HANDLE hSnapshot;
	DWORD dPid = 0;
	PROCESSENTRY32W pe32;
	int working;
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnapshot) {
		return 0;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	for (working = Process32FirstW(hSnapshot, &pe32); working; working = Process32NextW(hSnapshot, &pe32))
	{
		if (!wcscmp(pe32.szExeFile, process))
		{
			dPid = pe32.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return dPid;
}

HMODULE GetDLLHandle(wchar_t* wDllName, DWORD dPid)
{
	HMODULE result;
	tagMODULEENTRY32W me32;
	void* snapMod;

	if (!dPid) {
		return 0;
	}

	snapMod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dPid);
	me32.dwSize = sizeof(tagMODULEENTRY32W);
	if (Module32FirstW(snapMod, &me32))
	{
		while (wcscmp(wDllName, me32.szModule))
		{
			if (!Module32NextW(snapMod, &me32))
				goto error;
		}
		CloseHandle(snapMod);
		result = me32.hModule;
	}
	else
	{
	error:
		CloseHandle(snapMod);
		result = 0;
	}
	return result;
}

BOOL EnableDebugPrivilege() 
{
	HANDLE TokenHandle = NULL;
	TOKEN_PRIVILEGES TokenPrivilege;

	LUID uID;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle)) {
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID)) {
			TokenPrivilege.PrivilegeCount = 1;
			TokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			TokenPrivilege.Privileges[0].Luid = uID;
			if (AdjustTokenPrivileges(TokenHandle, FALSE, &TokenPrivilege, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
				CloseHandle(TokenHandle);
				TokenHandle = INVALID_HANDLE_VALUE;
				return TRUE;
			}
			else
				goto fail;

		}
		else
			goto fail;
	}
	else
		goto fail;

fail:
	CloseHandle(TokenHandle);
	TokenHandle = INVALID_HANDLE_VALUE;
	return FALSE;
}


static unsigned char GetProcAddressAsmCode[] = {
	0x55,                         // push ebp;
	0x8B, 0xEC,                   // mov ebp, esp;
	0x83, 0xEC, 0x40,             // sub esp, 0x40;
	0x57,                         // push edi;
	0x51,                         // push ecx;
	0x8B, 0x7D, 0x08,             // mov edi, dword ptr[ebp + 0x8];
	0x8B, 0x07,                   // mov eax,dword ptr[edi];
	0x50,                         // push eax;
	0xE8, 0x00, 0x00, 0x00, 0x00, // call GetModuleHandleW;
	0x83, 0xC4, 0x04,             // add esp,0x4;
	0x83, 0xC7, 0x04,             // add edi,0x4;
	0x8B, 0x0F,                   // mov ecx, dword ptr[edi];
	0x51,                         // push ecx;
	0x50,                         // push eax;
	0xE8, 0x00, 0x00, 0x00, 0x00, // call GetProcAddress;
	0x83, 0xC4, 0x08,             // add esp, 0x8;
	0x59,                         // pop ecx;
	0x5F,                         // pop edi;
	0x8B, 0xE5,                   // mov esp, ebp;
	0x5D,                         // pop ebp;
	0xC3                          // retn;
};

LPVOID FillAsmCode(HANDLE handle) {
	DWORD pGetModuleHandleW = (DWORD)GetModuleHandleW;
	DWORD pGetProcAddress = (DWORD)GetProcAddress;
	PVOID fillCall1 = (PVOID)&GetProcAddressAsmCode[15];
	PVOID fillCall2 = (PVOID)&GetProcAddressAsmCode[30];
	LPVOID pAsmFuncAddr = VirtualAllocEx(handle, NULL, 1, MEM_COMMIT, PAGE_EXECUTE);
	if (!pAsmFuncAddr) {
		return 0;
	}
	*(DWORD*)fillCall1 = pGetModuleHandleW - (DWORD)pAsmFuncAddr - 14 - 5;
	*(DWORD*)fillCall2 = pGetProcAddress - (DWORD)pAsmFuncAddr - 29 - 5;
	//*(DWORD*)fillCall1 = pGetModuleHandleW ;
	//*(DWORD*)fillCall2 = pGetProcAddress;
	SIZE_T dwWriteSize;
	WriteProcessMemory(handle, pAsmFuncAddr, GetProcAddressAsmCode, sizeof(GetProcAddressAsmCode), &dwWriteSize);
	return pAsmFuncAddr;

}


int  InjectDllAndStartHttp(wchar_t* szPName, wchar_t* szDllPath, DWORD port)
{
	if(!EnableDebugPrivilege()){
		return 0;
	}
	int result = 0;
	HANDLE hRemoteThread = NULL;
	LPTHREAD_START_ROUTINE lpSysLibAddr = NULL;
	HINSTANCE__* hKernelModule = NULL;
	LPVOID lpRemoteDllBase = NULL;
	HANDLE hProcess;
	unsigned int dwPid;
	size_t ulDllLength;
	wchar_t* dllName = (wchar_t*)L"wxhelper.dll";
	size_t dllNameLen = wcslen(dllName) * 2 + 2;
	char* funcName = (char* )"http_start";
	size_t funcNameLen = strlen(funcName) + 1;

	HANDLE  hStartHttp = NULL;
	LPVOID portAddr = NULL;
	HANDLE  getProcThread = NULL;

	LPVOID paramsAddr = NULL;
	LPVOID param1Addr = NULL;
	LPVOID param2Addr = NULL;
	LPVOID  GetProcFuncAddr = NULL;

	DWORD params[2] = { 0 };

	dwPid = GetPIDForProcess(szPName);
	ulDllLength = (wcslen(szDllPath) + 1) * sizeof(wchar_t);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	if (!hProcess) {
		goto error;
	}

	lpRemoteDllBase = VirtualAllocEx(hProcess, NULL, ulDllLength, MEM_COMMIT, PAGE_READWRITE);
	if (lpRemoteDllBase)
	{
		if (WriteProcessMemory(hProcess, lpRemoteDllBase, szDllPath, ulDllLength, NULL)
			&& (hKernelModule = GetModuleHandleW(L"kernel32.dll")) != 0
			&& (lpSysLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernelModule, "LoadLibraryW")) != 0
			&& (hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, lpSysLibAddr, lpRemoteDllBase, 0, NULL)) != 0)
		{
			WaitForSingleObject(hRemoteThread, INFINITE);
			GetProcFuncAddr = FillAsmCode(hProcess);
			param1Addr = VirtualAllocEx(hProcess, NULL, dllNameLen, MEM_COMMIT, PAGE_READWRITE);
			if (param1Addr) {
				SIZE_T dwWriteSize;
				BOOL  bRet = WriteProcessMemory(hProcess, (LPVOID)param1Addr, dllName, dllNameLen, &dwWriteSize);
				if (!bRet) {
					goto error;
				}
			}
			param2Addr = VirtualAllocEx(hProcess, NULL, funcNameLen, MEM_COMMIT, PAGE_READWRITE);
			if (param2Addr) {
				SIZE_T dwWriteSize;
				BOOL  bRet = WriteProcessMemory(hProcess, (LPVOID)param2Addr, funcName, funcNameLen, &dwWriteSize);
				if (!bRet) {
					goto error;
				}
			}

			params[0] = (DWORD)param1Addr;
			params[1] = (DWORD)param2Addr;

			paramsAddr = VirtualAllocEx(hProcess, NULL, sizeof(params), MEM_COMMIT, PAGE_READWRITE);
			if (paramsAddr) {
				SIZE_T dwWriteSize;
				BOOL  bRet = WriteProcessMemory(hProcess, (LPVOID)paramsAddr, &params[0], sizeof(params), &dwWriteSize);
				if (!bRet) {
					goto error;
				}
			}

			DWORD dwRet = 0;
			getProcThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcFuncAddr, paramsAddr, 0, NULL);

			if (getProcThread)
			{
				WaitForSingleObject(getProcThread, INFINITE);
				GetExitCodeThread(getProcThread, &dwRet);
				if (dwRet) {
					hStartHttp = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwRet, (LPVOID)port, 0, NULL);
					WaitForSingleObject(hStartHttp, INFINITE);
					result = 1;
				}
			}
		}
	}
error:
	if (hRemoteThread) {
		CloseHandle(hRemoteThread);
	}
	if (getProcThread) {
		CloseHandle(getProcThread);
	}
	if (hStartHttp) {
		CloseHandle(hStartHttp);
	}

	if (lpRemoteDllBase) {
		VirtualFreeEx(hProcess, lpRemoteDllBase, ulDllLength, MEM_DECOMMIT | MEM_RELEASE);
	}
	if (param1Addr) {
		VirtualFreeEx(hProcess, param1Addr, dllNameLen, MEM_DECOMMIT | MEM_RELEASE);
	}

	if (param2Addr) {
		VirtualFreeEx(hProcess, param1Addr, funcNameLen, MEM_DECOMMIT | MEM_RELEASE);
	}

	if (paramsAddr) {
		VirtualFreeEx(hProcess, param1Addr, sizeof(params), MEM_DECOMMIT | MEM_RELEASE);
	}

	if (GetProcFuncAddr) {
		VirtualFreeEx(hProcess, GetProcFuncAddr, sizeof(GetProcAddressAsmCode), MEM_DECOMMIT | MEM_RELEASE);
	}

	CloseHandle(hProcess);
	return result;
}

int  InjectDll(wchar_t* szPName, wchar_t* szDllPath)
{
	if(!EnableDebugPrivilege()){
		return 0;
	}
	int result = 0;
	HANDLE hRemoteThread;
	LPTHREAD_START_ROUTINE lpSysLibAddr;
	HINSTANCE__* hKernelModule;
	LPVOID lpRemoteDllBase;
	HANDLE hProcess;
	unsigned int dwPid;
	size_t ulDllLength;

	dwPid = GetPIDForProcess(szPName);
	ulDllLength = (wcslen(szDllPath) + 1) * sizeof(wchar_t);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPid);
	if (!hProcess) {
		return 0;
	}

	lpRemoteDllBase = VirtualAllocEx(hProcess, NULL, ulDllLength, MEM_COMMIT, PAGE_READWRITE);
	if (lpRemoteDllBase)
	{
		if (WriteProcessMemory(hProcess, lpRemoteDllBase, szDllPath, ulDllLength, NULL)
			&& (hKernelModule = GetModuleHandleW(L"kernel32.dll")) != 0
			&& (lpSysLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernelModule, "LoadLibraryW")) != 0
			&& (hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, lpSysLibAddr, lpRemoteDllBase, 0, NULL)) != 0)
		{
			WaitForSingleObject(hRemoteThread, INFINITE);
			VirtualFreeEx(hProcess, lpRemoteDllBase, ulDllLength, MEM_DECOMMIT | MEM_RELEASE);
			CloseHandle(hRemoteThread);
			CloseHandle(hProcess);
			OutputDebugStringA("[DBG] dll inject success");
			printf("dll inject success");
			printf("dll path : %s ", szDllPath);
			printf("dll path : %d ", dwPid);
			result = 1;
		}
		else
		{
			VirtualFreeEx(hProcess, lpRemoteDllBase, ulDllLength, MEM_DECOMMIT | MEM_RELEASE);
			CloseHandle(hProcess);
			result = 0;
		}
	}
	else
	{
		CloseHandle(hProcess);
		result = 0;
	}
	return result;
}

int  UnInjectDll(wchar_t* szPName, wchar_t* szDName)
{
	HMODULE  hDll;
	HANDLE lpFreeLibAddr;
	HINSTANCE__* hK32;
	HANDLE hProcess;
	unsigned int dwPID;

	dwPID = GetPIDForProcess(szPName);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwPID);
	if (!hProcess) {
		return 0;
	}

	hK32 = GetModuleHandleW(L"Kernel32.dll");
	if (!hK32) {
		return 0;
	}

	lpFreeLibAddr = GetProcAddress(hK32, "FreeLibraryAndExitThread");
	//lpFreeLibAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hK32, "FreeLibrary");
	hDll = GetDLLHandle(szDName, dwPID);
	if (hDll) {
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpFreeLibAddr, hDll, NULL, NULL);
		if (hThread == NULL) {
			int errorCode = GetLastError();
			return 0;
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return 1;
	}

	CloseHandle(hProcess);
	return 0;
}

FARPROC ShellCode(DWORD param[]) {
	return GetProcAddress(GetModuleHandleW((LPCWSTR)param[0]), (LPCSTR)param[1]);
}


int main(int argc, char** argv)
{
	int param;
	char cInjectprogram[MAX_PATH] = { 0 };
	char cUnInjectprogram[MAX_PATH] = { 0 };
	char cDllPath[MAX_PATH] = { 0 };
	char cDllName[MAX_PATH] = { 0 };
	int port = 0;

	ULONG pid = 0;

	while ((param = getopt(argc, argv, "i:p:u:d:m:P:h")) != -1)
	{
		switch (param)
		{
		case 'i':
			strcpy(cInjectprogram, optarg);
			break;
		case 'p':
			strcpy(cDllPath, optarg);
			break;
		case 'u':
			strcpy(cUnInjectprogram, optarg);
		case 'd':
			strcpy(cDllName, optarg);
			break;
		case 'h':
			printf("Usage: %s [-i/u]  [-p/d] [-m]\n", argv[0]);
			printf("Options:\n");
			printf("  -h         Print this help message.\n");
			printf("  -i <program name>   Name of the running program to be injected.\n");
			printf("  -u <program name>   Name of the running program to be uninstalled.\n");
			printf("  -p <path>   Full path of injection file.\n");
			printf("  -d <file>   Name of injection file.\n");
			printf("  -m <pid>   WeChat.exe  pid.\n");
			printf("\n");
			printf("Examples:\n");
			printf("  window>  %s -i test.exe -p c:/inject.dll \n", argv[0]);
			printf("  window>  %s -u test.exe -d inject.dll \n", argv[0]);
			printf("  window>  %s -m 1988 \n", argv[0]);
			exit(0);
			break;
		case 'm':
			pid = std::stol(optarg);
			break;
		case 'P':
			port = std::atoi(optarg);
			break;
		default:
			abort();
			break;
		}
	}

	if (pid) {
		FindHandles(pid, (LPSTR)"_WeChat_App_Instance_Identity_Mutex_Name", TRUE, TRUE);
	}

	if (cInjectprogram[0] != 0 && cDllPath[0] != 0)
	{
		if (cInjectprogram[0] != '\0' && cDllPath[0] != '\0')
		{
			if (port == 0) {
				std::wstring wsProgram = Utf8ToUnicode(cInjectprogram);
				std::wstring wsPath = Utf8ToUnicode(cDllPath);
				int ret = InjectDll((wchar_t*)wsProgram.c_str(), (wchar_t*)wsPath.c_str());
				printf(" 注入结果：%i \n", ret);
			}
			else
			{
				std::wstring wsProgram = Utf8ToUnicode(cInjectprogram);
				std::wstring wsPath = Utf8ToUnicode(cDllPath);
				int ret = InjectDllAndStartHttp((wchar_t*)wsProgram.c_str(), (wchar_t*)wsPath.c_str(), port);
				printf(" 注入结果：%i \n", ret);
			}
		}
	}

	if (cUnInjectprogram[0] != 0 && cDllName[0] != 0)
	{
		if (cUnInjectprogram[0] != '\0' && cDllName[0] != '\0')
		{
			std::wstring wsUnInjectProgram = Utf8ToUnicode(cUnInjectprogram);
			std::wstring wsName = Utf8ToUnicode(cDllName);
			int ret = UnInjectDll((wchar_t*)wsUnInjectProgram.c_str(), (wchar_t*)wsName.c_str());
			printf(" 卸载结果：%i \n", ret);
		}

	}

	return 0;
}
