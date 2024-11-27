#pragma once

#include "corjithost.h"
#include "corjit.h"

// This file defines all of the methods a JIT/EE interface can define.
// It serves to create a friendly abstraction for languages other than C++.
//
// For a large number of use cases, a lot of these functions can be left
// un-implemented. For specifics on what these functions should do,
// look into the ICorJitInfo (corjit.h), ICorDynamicInfo (corinfo.h),
// and ICorStaticInfo (corinfo.h).
//
// You can also take a look at how .NET implements them for Native AOT or the VM.

#define SELF_PTR void*

// Defines methods defined on a `ICorJitInfo` object. Fields of this struct may
// be NULL, in which case the method will be assumed to not be implemented.
typedef struct {
    // ===================================================
    // ICorJitInfo
    // ===================================================

    void (*allocMem)(SELF_PTR, AllocMemArgs*);
    void (*reserveUnwindInfo)(SELF_PTR, bool, bool, uint32_t);
    void (*allocUnwindInfo)(SELF_PTR, uint8_t*, uint8_t*, uint32_t, uint32_t, uint32_t, uint8_t*, CorJitFuncKind);
    void* (*allocGCInfo)(SELF_PTR, size_t);
    void (*setEHcount)(SELF_PTR, unsigned);
    void (*setEHinfo)(SELF_PTR, unsigned, const CORINFO_EH_CLAUSE*);
    bool (*logMsg)(SELF_PTR, unsigned, const char*, va_list);
    int (*doAssert)(SELF_PTR, const char*, int, const char*);
    void (*reportFatalError)(SELF_PTR, CorJitResult);

    JITINTERFACE_HRESULT (*getPgoInstrumentationResults)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        ICorJitInfo::PgoInstrumentationSchema**,
        uint32_t*,
        uint8_t**,
        ICorJitInfo::PgoSource*,
        bool*
    );

    JITINTERFACE_HRESULT (*allocPgoInstrumentationBySchema)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        ICorJitInfo::PgoInstrumentationSchema*,
        uint32_t,
        uint8_t**
    );

    void (*recordCallSite)(SELF_PTR, uint32_t, CORINFO_SIG_INFO*, CORINFO_METHOD_HANDLE);
    void (*recordRelocation)(SELF_PTR, void*, void*, void*, uint16_t, int32_t);
    uint16_t (*getRelocTypeHint)(SELF_PTR, void*);
    uint32_t (*getExpectedTargetArchitecture)(SELF_PTR);
    uint32_t (*getJitFlags)(SELF_PTR, CORJIT_FLAGS*, uint32_t);

    // ===================================================
    // ICorDynamicInfo
    // ===================================================

    uint32_t (*getThreadTLSIndex)(SELF_PTR, void**);
    int32_t* (*getAddrOfCaptureThreadGlobal)(SELF_PTR, void**);
    void* (*getHelperFtn)(SELF_PTR, CorInfoHelpFunc, void**);
    void (*getFunctionEntryPoint)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_CONST_LOOKUP*, CORINFO_ACCESS_FLAGS);
    void (*getFunctionFixedEntryPoint)(SELF_PTR, CORINFO_METHOD_HANDLE, bool, CORINFO_CONST_LOOKUP*);
    void* (*getMethodSync)(SELF_PTR, CORINFO_METHOD_HANDLE, void**);
    CorInfoHelpFunc (*getLazyStringLiteralHelper)(SELF_PTR, CORINFO_MODULE_HANDLE);
    CORINFO_MODULE_HANDLE (*embedModuleHandle)(SELF_PTR, CORINFO_MODULE_HANDLE, void**);
    CORINFO_CLASS_HANDLE (*embedClassHandle)(SELF_PTR, CORINFO_CLASS_HANDLE, void**);
    CORINFO_METHOD_HANDLE (*embedMethodHandle)(SELF_PTR, CORINFO_METHOD_HANDLE, void**);
    CORINFO_FIELD_HANDLE (*embedFieldHandle)(SELF_PTR, CORINFO_FIELD_HANDLE, void**);

    void (*embedGenericHandle)(SELF_PTR,
        CORINFO_RESOLVED_TOKEN*,
        bool,
        CORINFO_METHOD_HANDLE,
        CORINFO_GENERICHANDLE_RESULT*
    );

    void (*getLocationOfThisType)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_LOOKUP_KIND*);
    void (*getAddressOfPInvokeTarget)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_CONST_LOOKUP*);
    void* (*GetCookieForPInvokeCalliSig)(SELF_PTR, CORINFO_SIG_INFO*, void**);
    bool (*canGetCookieForPInvokeCalliSig)(SELF_PTR, CORINFO_SIG_INFO* szMetaSig);
    CORINFO_JUST_MY_CODE_HANDLE (*getJustMyCodeHandle)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_JUST_MY_CODE_HANDLE**);
    void (*GetProfilingHandle)(SELF_PTR, bool*, void**, bool*);

    void (*getCallInfo)(SELF_PTR,
        CORINFO_RESOLVED_TOKEN*,
        CORINFO_RESOLVED_TOKEN*,
        CORINFO_METHOD_HANDLE,
        CORINFO_CALLINFO_FLAGS,
        CORINFO_CALL_INFO*
    );

    bool (*getStaticFieldContent)(SELF_PTR, CORINFO_FIELD_HANDLE, uint8_t*, int, int, bool);
    bool (*getObjectContent)(SELF_PTR, CORINFO_OBJECT_HANDLE, uint8_t*, int, int);
    CORINFO_CLASS_HANDLE (*getStaticFieldCurrentClass)(SELF_PTR, CORINFO_FIELD_HANDLE, bool*);
    CORINFO_VARARGS_HANDLE (*getVarArgsHandle)(SELF_PTR, CORINFO_SIG_INFO*, void**);
    bool (*canGetVarArgsHandle)(SELF_PTR, CORINFO_SIG_INFO*);
    InfoAccessType (*constructStringLiteral)(SELF_PTR, CORINFO_MODULE_HANDLE, mdToken, void**);
    InfoAccessType (*emptyStringLiteral)(SELF_PTR, void**);
    uint32_t (*getFieldThreadLocalStoreID)(SELF_PTR, CORINFO_FIELD_HANDLE, void**);

    CORINFO_METHOD_HANDLE (*GetDelegateCtor)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        CORINFO_CLASS_HANDLE,
        CORINFO_METHOD_HANDLE,
        DelegateCtorArgs*
    );

    void (*MethodCompileComplete)(SELF_PTR, CORINFO_METHOD_HANDLE);

    bool (*getTailCallHelpers)(SELF_PTR,
        CORINFO_RESOLVED_TOKEN*,
        CORINFO_SIG_INFO*,
        CORINFO_GET_TAILCALL_HELPERS_FLAGS,
        CORINFO_TAILCALL_HELPERS*
    );

    bool (*convertPInvokeCalliToCall)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, bool);
    bool (*notifyInstructionSetUsage)(SELF_PTR, CORINFO_InstructionSet, bool);
    void (*updateEntryPointForTailCall)(SELF_PTR, CORINFO_CONST_LOOKUP*);
    CORINFO_METHOD_HANDLE (*getSpecialCopyHelper)(SELF_PTR, CORINFO_CLASS_HANDLE);

    // ===================================================
    // ICorStaticInfo
    // ===================================================
    bool (*isIntrinsic)(SELF_PTR, CORINFO_METHOD_HANDLE);
    bool (*notifyMethodInfoUsage)(SELF_PTR, CORINFO_METHOD_HANDLE);
    uint32_t (*getMethodAttribs)(SELF_PTR, CORINFO_METHOD_HANDLE);
    void (*setMethodAttribs)(SELF_PTR, CORINFO_METHOD_HANDLE, CorInfoMethodRuntimeFlags);
    void (*getMethodSig)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_SIG_INFO*, CORINFO_CLASS_HANDLE);
    bool (*getMethodInfo)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_METHOD_INFO*, CORINFO_CONTEXT_HANDLE);
    bool (*haveSameMethodDefinition)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_METHOD_HANDLE);
    CORINFO_CLASS_HANDLE (*getTypeDefinition)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoInline (*canInline)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_METHOD_HANDLE);
    void (*beginInlining)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_METHOD_HANDLE);

    void (*reportInliningDecision)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        CORINFO_METHOD_HANDLE,
        CorInfoInline,
        const char*
    );

    bool (*canTailCall)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        CORINFO_METHOD_HANDLE,
        CORINFO_METHOD_HANDLE,
        bool
    );

    void (*reportTailCallDecision)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        CORINFO_METHOD_HANDLE,
        bool,
        CorInfoTailCall,
        const char *
    );

    void (*getEHinfo)(SELF_PTR, CORINFO_METHOD_HANDLE, unsigned, CORINFO_EH_CLAUSE*);
    CORINFO_CLASS_HANDLE (*getMethodClass)(SELF_PTR, CORINFO_METHOD_HANDLE);
    void (*getMethodVTableOffset)(SELF_PTR, CORINFO_METHOD_HANDLE, unsigned*, unsigned*, bool*);
    bool (*resolveVirtualMethod)(SELF_PTR, CORINFO_DEVIRTUALIZATION_INFO* info);
    CORINFO_METHOD_HANDLE (*getUnboxedEntry)(SELF_PTR, CORINFO_METHOD_HANDLE, bool*);
    CORINFO_METHOD_HANDLE (*getInstantiatedEntry)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_METHOD_HANDLE*, CORINFO_CLASS_HANDLE*);
    CORINFO_CLASS_HANDLE (*getDefaultComparerClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_CLASS_HANDLE (*getDefaultEqualityComparerClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_CLASS_HANDLE (*getSZArrayHelperEnumeratorClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    void (*expandRawHandleIntrinsic)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, CORINFO_METHOD_HANDLE, CORINFO_GENERICHANDLE_RESULT*);
    bool (*isIntrinsicType)(SELF_PTR, CORINFO_CLASS_HANDLE classHnd); // (has default impl)

    CorInfoCallConvExtension (*getUnmanagedCallConv)(SELF_PTR,
        CORINFO_METHOD_HANDLE,
        CORINFO_SIG_INFO*,
        bool*
    );

    bool (*pInvokeMarshalingRequired)(SELF_PTR, CORINFO_METHOD_HANDLE, CORINFO_SIG_INFO*);
    bool (*satisfiesMethodConstraints)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_METHOD_HANDLE);
    void (*methodMustBeLoadedBeforeCodeIsRun)(SELF_PTR, CORINFO_METHOD_HANDLE);
    void (*getGSCookie)(SELF_PTR, GSCookie*, GSCookie**);
    void (*setPatchpointInfo)(SELF_PTR, PatchpointInfo*);
    PatchpointInfo* (*getOSRInfo)(SELF_PTR, unsigned*);
    void (*resolveToken)(SELF_PTR, CORINFO_RESOLVED_TOKEN*);
    void (*findSig)(SELF_PTR, CORINFO_MODULE_HANDLE, unsigned, CORINFO_CONTEXT_HANDLE, CORINFO_SIG_INFO*);
    void (*findCallSiteSig)(SELF_PTR, CORINFO_MODULE_HANDLE, unsigned, CORINFO_CONTEXT_HANDLE, CORINFO_SIG_INFO*);
    CORINFO_CLASS_HANDLE (*getTokenTypeAsHandle)(SELF_PTR, CORINFO_RESOLVED_TOKEN*);
    int (*getStringLiteral)(SELF_PTR, CORINFO_MODULE_HANDLE, unsigned, char16_t*, int, int);
    size_t (*printObjectDescription)(SELF_PTR, CORINFO_OBJECT_HANDLE, char*, size_t, size_t*);
    CorInfoType (*asCorInfoType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    const char* (*getClassNameFromMetadata)(SELF_PTR, CORINFO_CLASS_HANDLE, const char**);
    CORINFO_CLASS_HANDLE (*getTypeInstantiationArgument)(SELF_PTR, CORINFO_CLASS_HANDLE, unsigned);
    CORINFO_CLASS_HANDLE (*getMethodInstantiationArgument)(SELF_PTR, CORINFO_METHOD_HANDLE, unsigned);
    size_t (*printClassName)(SELF_PTR, CORINFO_CLASS_HANDLE, char*, size_t, size_t*);
    bool (*isValueClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    uint32_t (*getClassAttribs)(SELF_PTR, CORINFO_CLASS_HANDLE);
    const char* (*getClassAssemblyName)(SELF_PTR, CORINFO_CLASS_HANDLE);
    void* (*LongLifetimeMalloc)(SELF_PTR, size_t);
    void (*LongLifetimeFree)(SELF_PTR, void*);
    bool (*getIsClassInitedFlagAddress)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CONST_LOOKUP*, int*);
    size_t (*getClassStaticDynamicInfo)(SELF_PTR, CORINFO_CLASS_HANDLE);
    size_t (*getClassThreadStaticDynamicInfo)(SELF_PTR, CORINFO_CLASS_HANDLE);
    bool (*getStaticBaseAddress)(SELF_PTR, CORINFO_CLASS_HANDLE, bool, CORINFO_CONST_LOOKUP*);
    unsigned (*getClassSize)(SELF_PTR, CORINFO_CLASS_HANDLE);
    unsigned (*getHeapClassSize)(SELF_PTR, CORINFO_CLASS_HANDLE);
    bool (*canAllocateOnStack)(SELF_PTR, CORINFO_CLASS_HANDLE);
    unsigned (*getClassAlignmentRequirement)(SELF_PTR, CORINFO_CLASS_HANDLE, bool);
    unsigned (*getClassGClayout)(SELF_PTR, CORINFO_CLASS_HANDLE, uint8_t*);
    unsigned (*getClassNumInstanceFields)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_FIELD_HANDLE (*getFieldInClass)(SELF_PTR, CORINFO_CLASS_HANDLE, int32_t);
    GetTypeLayoutResult (*getTypeLayout)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_TYPE_LAYOUT_NODE*, size_t*);
    bool (*checkMethodModifier)(SELF_PTR, CORINFO_METHOD_HANDLE, const char*, bool);
    CorInfoHelpFunc (*getNewHelper)(SELF_PTR, CORINFO_CLASS_HANDLE, bool*);
    CorInfoHelpFunc (*getNewArrHelper)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoHelpFunc (*getCastingHelper)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, bool);
    CorInfoHelpFunc (*getSharedCCtorHelper)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_CLASS_HANDLE (*getTypeForBox)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_CLASS_HANDLE (*getTypeForBoxOnStack)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoHelpFunc (*getBoxHelper)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoHelpFunc (*getUnBoxHelper)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_OBJECT_HANDLE (*getRuntimeTypePointer)(SELF_PTR, CORINFO_CLASS_HANDLE);
    bool (*isObjectImmutable)(SELF_PTR, CORINFO_OBJECT_HANDLE);
    bool (*getStringChar)(SELF_PTR, CORINFO_OBJECT_HANDLE, int, uint16_t*);
    CORINFO_CLASS_HANDLE (*getObjectType)(SELF_PTR, CORINFO_OBJECT_HANDLE);
    bool (*getReadyToRunHelper)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, CORINFO_LOOKUP_KIND*, CorInfoHelpFunc, CORINFO_METHOD_HANDLE, CORINFO_CONST_LOOKUP*);
    void (*getReadyToRunDelegateCtorHelper)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, mdToken, CORINFO_CLASS_HANDLE, CORINFO_METHOD_HANDLE, CORINFO_LOOKUP*);
    CorInfoInitClassResult (*initClass)(SELF_PTR, CORINFO_FIELD_HANDLE, CORINFO_METHOD_HANDLE, CORINFO_CONTEXT_HANDLE);
    void (*classMustBeLoadedBeforeCodeIsRun)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CORINFO_CLASS_HANDLE (*getBuiltinClass)(SELF_PTR, CorInfoClassId);
    CorInfoType (*getTypeForPrimitiveValueClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoType (*getTypeForPrimitiveNumericClass)(SELF_PTR, CORINFO_CLASS_HANDLE);
    bool (*canCast)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE);
    TypeCompareState (*compareTypesForCast)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE);
    TypeCompareState (*compareTypesForEquality)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE);
    bool (*isMoreSpecificType)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE);
    bool (*isExactType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    TypeCompareState (*isGenericType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    TypeCompareState (*isNullableType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    TypeCompareState (*isEnum)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE*);
    CORINFO_CLASS_HANDLE (*getParentType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoType (*getChildType)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_CLASS_HANDLE*);
    bool (*isSDArray)(SELF_PTR, CORINFO_CLASS_HANDLE);
    unsigned (*getArrayRank)(SELF_PTR, CORINFO_CLASS_HANDLE);
    CorInfoArrayIntrinsic (*getArrayIntrinsicID)(SELF_PTR, CORINFO_METHOD_HANDLE);
    void* (*getArrayInitializationData)(SELF_PTR, CORINFO_FIELD_HANDLE, uint32_t);
    CorInfoIsAccessAllowedResult (*canAccessClass)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, CORINFO_METHOD_HANDLE, CORINFO_HELPER_DESC*);
    size_t (*printFieldName)(SELF_PTR, CORINFO_FIELD_HANDLE, char*, size_t, size_t*);
    CORINFO_CLASS_HANDLE (*getFieldClass)(SELF_PTR, CORINFO_FIELD_HANDLE);
    CorInfoType (*getFieldType)(SELF_PTR, CORINFO_FIELD_HANDLE, CORINFO_CLASS_HANDLE*, CORINFO_CLASS_HANDLE);
    unsigned (*getFieldOffset)(SELF_PTR, CORINFO_FIELD_HANDLE);
    void (*getFieldInfo)(SELF_PTR, CORINFO_RESOLVED_TOKEN*, CORINFO_METHOD_HANDLE, CORINFO_ACCESS_FLAGS, CORINFO_FIELD_INFO*);
    uint32_t (*getThreadLocalFieldInfo)(SELF_PTR, CORINFO_FIELD_HANDLE, bool);
    void (*getThreadLocalStaticBlocksInfo)(SELF_PTR, CORINFO_THREAD_STATIC_BLOCKS_INFO*);
    void (*getThreadLocalStaticInfo_NativeAOT)(SELF_PTR, CORINFO_THREAD_STATIC_INFO_NATIVEAOT*);
    bool (*isFieldStatic)(SELF_PTR, CORINFO_FIELD_HANDLE);
    int (*getArrayOrStringLength)(SELF_PTR, CORINFO_OBJECT_HANDLE);
    void (*getBoundaries)(SELF_PTR, CORINFO_METHOD_HANDLE, unsigned int*, uint32_t**, ICorDebugInfo::BoundaryTypes*);
    void (*setBoundaries)(SELF_PTR, CORINFO_METHOD_HANDLE, uint32_t, ICorDebugInfo::OffsetMapping*);
    void (*getVars)(SELF_PTR, CORINFO_METHOD_HANDLE, uint32_t*, ICorDebugInfo::ILVarInfo**, bool*);
    void (*setVars)(SELF_PTR, CORINFO_METHOD_HANDLE, uint32_t, ICorDebugInfo::NativeVarInfo*);
    void (*reportRichMappings)(SELF_PTR, ICorDebugInfo::InlineTreeNode*, uint32_t, ICorDebugInfo::RichOffsetMapping*, uint32_t);
    void (*reportMetadata)(SELF_PTR, const char*, const void*, size_t);
    void* (*allocateArray)(SELF_PTR, size_t);
    void (*freeArray)(SELF_PTR, void*);
    CORINFO_ARG_LIST_HANDLE (*getArgNext)(SELF_PTR, CORINFO_ARG_LIST_HANDLE);
    CorInfoTypeWithMod (*getArgType)(SELF_PTR, CORINFO_SIG_INFO*, CORINFO_ARG_LIST_HANDLE, CORINFO_CLASS_HANDLE*);
    int (*getExactClasses)(SELF_PTR, CORINFO_CLASS_HANDLE, int, CORINFO_CLASS_HANDLE*);
    CORINFO_CLASS_HANDLE (*getArgClass)(SELF_PTR, CORINFO_SIG_INFO*, CORINFO_ARG_LIST_HANDLE);
    CorInfoHFAElemType (*getHFAType)(SELF_PTR, CORINFO_CLASS_HANDLE);
    bool (*runWithErrorTrap)(SELF_PTR, ICorStaticInfo::errorTrapFunction, void*);
    bool (*runWithSPMIErrorTrap)(SELF_PTR, ICorStaticInfo::errorTrapFunction, void*);
    void (*getEEInfo)(SELF_PTR, CORINFO_EE_INFO*);
    mdMethodDef (*getMethodDefFromMethod)(SELF_PTR, CORINFO_METHOD_HANDLE);
    size_t (*printMethodName)(SELF_PTR, CORINFO_METHOD_HANDLE, char*, size_t, size_t*);
    const char* (*getMethodNameFromMetadata)(SELF_PTR, CORINFO_METHOD_HANDLE, const char**, const char**, const char**, size_t);
    unsigned (*getMethodHash)(SELF_PTR, CORINFO_METHOD_HANDLE);
    bool (*getSystemVAmd64PassStructInRegisterDescriptor)(SELF_PTR, CORINFO_CLASS_HANDLE, SYSTEMV_AMD64_CORINFO_STRUCT_REG_PASSING_DESCRIPTOR*);
    void (*getSwiftLowering)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_SWIFT_LOWERING*);
    void (*getFpStructLowering)(SELF_PTR, CORINFO_CLASS_HANDLE, CORINFO_FPSTRUCT_LOWERING*);
} JitInfoMethods;

#define FORWARD_METHOD(returnType, methodName, signature, ...)  \
    returnType methodName signature override {                  \
        if (methods.methodName == nullptr) {                    \
            reportNotImplemented(#methodName);                  \
        }                                                       \
        return methods.methodName(self, __VA_ARGS__);           \
    }

#define FORWARD_METHOD_NOARG(returnType, methodName)     \
    returnType methodName() override {                   \
        if (methods.methodName == nullptr) {             \
            reportNotImplemented(#methodName);           \
        }                                                \
        return methods.methodName(self);                 \
    }

// Provides a `ICorJitInfo` implementation that is portable across multiple languages.
class UniversalJitInfo : public ICorJitInfo
{
public:
    // An arbitrary pointer. Passed to all functions from `JitInfoMethods`.
    void* self;

    // Reports that the JIT compiler attempted to invoke an `ICorJitInfo` method
    // that was not implemented (is `NULL` in the `methods` field). This function
    // should never return once invoked.
    void (*reportNotImplemented)(const char* methodName);

    // Provides a table of methods to redirect calls to the methods of this object.
    JitInfoMethods methods;

    UniversalJitInfo(
        void* self,
        void (*reportNotImplemented)(const char* methodName),
        JitInfoMethods* methods
    ) {
        this->self = self;
        this->reportNotImplemented = reportNotImplemented;
        this->methods = *methods;
    }

    // ===================================================
    // ICorJitInfo
    // ===================================================
    FORWARD_METHOD(
        void, allocMem,
        (AllocMemArgs* args),
        args
    );

    FORWARD_METHOD(
        void, reserveUnwindInfo,
        (bool isFunclet, bool isColdCode, uint32_t unwindSize),
        isFunclet, isColdCode, unwindSize
    );

    FORWARD_METHOD(
        void, allocUnwindInfo,
        (uint8_t* pHotCode, uint8_t* pColdCode, uint32_t startOffset, uint32_t endOffset, uint32_t unwindSize, uint8_t* pUnwindBlock, CorJitFuncKind funcKind),
        pHotCode, pColdCode, startOffset, endOffset, unwindSize, pUnwindBlock, funcKind
    );

    FORWARD_METHOD(
        void*, allocGCInfo,
        (size_t size),
        size
    );

    FORWARD_METHOD(
        void, setEHcount,
        (unsigned cEH),
        cEH
    );

    FORWARD_METHOD(
        void, setEHinfo,
        (unsigned EHnumber, const CORINFO_EH_CLAUSE* clause),
        EHnumber, clause
    );

    FORWARD_METHOD(
        bool, logMsg,
        (unsigned level, const char* fmt, va_list args),
        level, fmt, args
    );

    FORWARD_METHOD(
        int, doAssert,
        (const char* szFile, int iLine, const char* szExpr),
        szFile, iLine, szExpr
    );

    FORWARD_METHOD(
        void, reportFatalError,
        (CorJitResult result),
        result
    );

    FORWARD_METHOD(
        JITINTERFACE_HRESULT, getPgoInstrumentationResults,
        (CORINFO_METHOD_HANDLE ftnHnd, ICorJitInfo::PgoInstrumentationSchema** pSchema,
        uint32_t* pCountSchemaItems, uint8_t** pInstrumentationData,
        ICorJitInfo::PgoSource* pPgoSource, bool* pDynamicPgo),
        ftnHnd, pSchema, pCountSchemaItems, pInstrumentationData, pPgoSource, pDynamicPgo
    );

    FORWARD_METHOD(
        JITINTERFACE_HRESULT, allocPgoInstrumentationBySchema,
        (CORINFO_METHOD_HANDLE ftnHnd, ICorJitInfo::PgoInstrumentationSchema* pSchema,
        uint32_t countSchemaItems, uint8_t** pInstrumentationData),
        ftnHnd, pSchema, countSchemaItems, pInstrumentationData
    );

    FORWARD_METHOD(
        void, recordCallSite,
        (uint32_t instrOffset, CORINFO_SIG_INFO* callSig, CORINFO_METHOD_HANDLE methodHandle),
        instrOffset, callSig, methodHandle
    );

    FORWARD_METHOD(
        void, recordRelocation,
        (void* location, void* locationRW, void* target, uint16_t fRelocType, int32_t addlDelta),
        location, locationRW, target, fRelocType, addlDelta
    );

    FORWARD_METHOD(
        uint16_t, getRelocTypeHint,
        (void* target),
        target
    );

    FORWARD_METHOD_NOARG(uint32_t, getExpectedTargetArchitecture);

    FORWARD_METHOD(
        uint32_t, getJitFlags,
        (CORJIT_FLAGS* flags, uint32_t sizeInBytes),
        flags, sizeInBytes
    );

    // ===================================================
    // ICorDynamicInfo
    // ===================================================
    FORWARD_METHOD(
        uint32_t, getThreadTLSIndex,
        (void** ppIndirection),
        ppIndirection
    );

    FORWARD_METHOD(
        int32_t*, getAddrOfCaptureThreadGlobal,
        (void** ppIndirection),
        ppIndirection
    );

    FORWARD_METHOD(
        void*, getHelperFtn,
        (CorInfoHelpFunc ftnNum, void** ppIndirection),
        ftnNum, ppIndirection
    );

    FORWARD_METHOD(
        void, getFunctionEntryPoint,
        (CORINFO_METHOD_HANDLE ftn, CORINFO_CONST_LOOKUP* pResult, CORINFO_ACCESS_FLAGS accessFlags),
        ftn, pResult, accessFlags
    );

    FORWARD_METHOD(
        void, getFunctionFixedEntryPoint,
        (CORINFO_METHOD_HANDLE ftn, bool isUnsafeFunctionPointer, CORINFO_CONST_LOOKUP* pResult),
        ftn, isUnsafeFunctionPointer, pResult
    );

    FORWARD_METHOD(
        void*, getMethodSync,
        (CORINFO_METHOD_HANDLE ftn, void** ppIndirection),
        ftn, ppIndirection
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getLazyStringLiteralHelper,
        (CORINFO_MODULE_HANDLE handle),
        handle
    );

    FORWARD_METHOD(
        CORINFO_MODULE_HANDLE, embedModuleHandle,
        (CORINFO_MODULE_HANDLE handle, void** ppIndirection),
        handle, ppIndirection
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, embedClassHandle,
        (CORINFO_CLASS_HANDLE handle, void** ppIndirection),
        handle, ppIndirection
    );

    FORWARD_METHOD(
        CORINFO_METHOD_HANDLE, embedMethodHandle,
        (CORINFO_METHOD_HANDLE handle, void** ppIndirection),
        handle, ppIndirection
    );

    FORWARD_METHOD(
        CORINFO_FIELD_HANDLE, embedFieldHandle,
        (CORINFO_FIELD_HANDLE handle, void** ppIndirection),
        handle, ppIndirection
    );

    FORWARD_METHOD(
        void, embedGenericHandle,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, bool fEmbedParent, CORINFO_METHOD_HANDLE callerHandle, CORINFO_GENERICHANDLE_RESULT* pResult),
        pResolvedToken, fEmbedParent, callerHandle, pResult
    );

    FORWARD_METHOD(
        void, getLocationOfThisType,
        (CORINFO_METHOD_HANDLE context, CORINFO_LOOKUP_KIND* pLookupKind),
        context, pLookupKind
    );

    FORWARD_METHOD(
        void, getAddressOfPInvokeTarget,
        (CORINFO_METHOD_HANDLE method, CORINFO_CONST_LOOKUP* pLookup),
        method, pLookup
    );

    FORWARD_METHOD(
        void*, GetCookieForPInvokeCalliSig,
        (CORINFO_SIG_INFO* szMetaSig, void** ppIndirection),
        szMetaSig, ppIndirection
    );

    FORWARD_METHOD(
        bool, canGetCookieForPInvokeCalliSig,
        (CORINFO_SIG_INFO* szMetaSig),
        szMetaSig
    );

    FORWARD_METHOD(
        CORINFO_JUST_MY_CODE_HANDLE, getJustMyCodeHandle,
        (CORINFO_METHOD_HANDLE method, CORINFO_JUST_MY_CODE_HANDLE** ppIndirection),
        method, ppIndirection
    );

    FORWARD_METHOD(
        void, GetProfilingHandle,
        (bool* pbHookFunction, void** pProfilerHandle, bool* pbIndirectedHandles),
        pbHookFunction, pProfilerHandle, pbIndirectedHandles
    );

    FORWARD_METHOD(
        void, getCallInfo,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, CORINFO_RESOLVED_TOKEN* pConstrainedResolvedToken,
        CORINFO_METHOD_HANDLE callerHandle, CORINFO_CALLINFO_FLAGS flags, CORINFO_CALL_INFO* pResult),
        pResolvedToken, pConstrainedResolvedToken, callerHandle, flags, pResult
    );

    FORWARD_METHOD(
        bool, getStaticFieldContent,
        (CORINFO_FIELD_HANDLE field, uint8_t* buffer, int bufferSize, int valueOffset, bool ignoreMovableObjects),
        field, buffer, bufferSize, valueOffset, ignoreMovableObjects
    );

    FORWARD_METHOD(
        bool, getObjectContent,
        (CORINFO_OBJECT_HANDLE obj, uint8_t* buffer, int bufferSize, int valueOffset),
        obj, buffer, bufferSize, valueOffset
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getStaticFieldCurrentClass,
        (CORINFO_FIELD_HANDLE field, bool* pIsSpeculative),
        field, pIsSpeculative
    );

    FORWARD_METHOD(
        CORINFO_VARARGS_HANDLE, getVarArgsHandle,
        (CORINFO_SIG_INFO* pSig, void** ppIndirection),
        pSig, ppIndirection
    );

    FORWARD_METHOD(
        bool, canGetVarArgsHandle,
        (CORINFO_SIG_INFO* pSig),
        pSig
    );

    FORWARD_METHOD(
        InfoAccessType, constructStringLiteral,
        (CORINFO_MODULE_HANDLE module, mdToken metaTok, void** ppValue),
        module, metaTok, ppValue
    );

    FORWARD_METHOD(
        InfoAccessType, emptyStringLiteral,
        (void** ppValue),
        ppValue
    );

    FORWARD_METHOD(
        uint32_t, getFieldThreadLocalStoreID,
        (CORINFO_FIELD_HANDLE field, void** ppIndirection),
        field, ppIndirection
    );

    FORWARD_METHOD(
        CORINFO_METHOD_HANDLE, GetDelegateCtor,
        (CORINFO_METHOD_HANDLE methHnd, CORINFO_CLASS_HANDLE clsHnd, CORINFO_METHOD_HANDLE targetMethodHnd, DelegateCtorArgs* pCtorData),
        methHnd, clsHnd, targetMethodHnd, pCtorData
    );

    FORWARD_METHOD(
        void, MethodCompileComplete,
        (CORINFO_METHOD_HANDLE methHnd),
        methHnd
    );

    FORWARD_METHOD(
        bool, getTailCallHelpers,
        (CORINFO_RESOLVED_TOKEN* callToken, CORINFO_SIG_INFO* sig, CORINFO_GET_TAILCALL_HELPERS_FLAGS flags, CORINFO_TAILCALL_HELPERS* pResult),
        callToken, sig, flags, pResult
    );

    FORWARD_METHOD(
        bool, convertPInvokeCalliToCall,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, bool fMustConvert),
        pResolvedToken, fMustConvert
    );

    FORWARD_METHOD(
        bool, notifyInstructionSetUsage,
        (CORINFO_InstructionSet instructionSet, bool supportEnabled),
        instructionSet, supportEnabled
    );

    FORWARD_METHOD(
        void, updateEntryPointForTailCall,
        (CORINFO_CONST_LOOKUP* entryPoint),
        entryPoint
    );

    FORWARD_METHOD(
        CORINFO_METHOD_HANDLE, getSpecialCopyHelper,
        (CORINFO_CLASS_HANDLE type),
        type
    );

    // ===================================================
    // ICorStaticInfo
    // ===================================================
    FORWARD_METHOD(
        bool, isIntrinsic,
        (CORINFO_METHOD_HANDLE ftn),
        ftn
    );

    FORWARD_METHOD(
        bool, notifyMethodInfoUsage,
        (CORINFO_METHOD_HANDLE ftn),
        ftn
    );

    FORWARD_METHOD(
        uint32_t, getMethodAttribs,
        (CORINFO_METHOD_HANDLE ftn),
        ftn
    );

    FORWARD_METHOD(
        void, setMethodAttribs,
        (CORINFO_METHOD_HANDLE ftn, CorInfoMethodRuntimeFlags attribs),
        ftn, attribs
    );

    FORWARD_METHOD(
        void, getMethodSig,
        (CORINFO_METHOD_HANDLE ftn, CORINFO_SIG_INFO* sig, CORINFO_CLASS_HANDLE memberParent),
        ftn, sig, memberParent
    );

    FORWARD_METHOD(
        bool, getMethodInfo,
        (CORINFO_METHOD_HANDLE ftn, CORINFO_METHOD_INFO* info, CORINFO_CONTEXT_HANDLE context),
        ftn, info, context
    );

    FORWARD_METHOD(
        bool, haveSameMethodDefinition,
        (CORINFO_METHOD_HANDLE meth1Hnd, CORINFO_METHOD_HANDLE meth2Hnd),
        meth1Hnd, meth2Hnd
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getTypeDefinition,
        (CORINFO_CLASS_HANDLE type),
        type
    );

    FORWARD_METHOD(
        CorInfoInline, canInline,
        (CORINFO_METHOD_HANDLE callerHnd, CORINFO_METHOD_HANDLE calleeHnd),
        callerHnd, calleeHnd
    );

    FORWARD_METHOD(
        void, beginInlining,
        (CORINFO_METHOD_HANDLE inlinerHnd, CORINFO_METHOD_HANDLE inlineeHnd),
        inlinerHnd, inlineeHnd
    );

    FORWARD_METHOD(
        void, reportInliningDecision,
        (CORINFO_METHOD_HANDLE inlinerHnd, CORINFO_METHOD_HANDLE inlineeHnd, CorInfoInline inlineResult, const char* reason),
        inlinerHnd, inlineeHnd, inlineResult, reason
    );

    FORWARD_METHOD(
        bool, canTailCall,
        (CORINFO_METHOD_HANDLE callerHnd, CORINFO_METHOD_HANDLE declaredCalleeHnd,
        CORINFO_METHOD_HANDLE exactCalleeHnd, bool fIsTailPrefix),
        callerHnd, declaredCalleeHnd, exactCalleeHnd, fIsTailPrefix
    );

    FORWARD_METHOD(
        void, reportTailCallDecision,
        (CORINFO_METHOD_HANDLE callerHnd, CORINFO_METHOD_HANDLE calleeHnd,
        bool fIsTailPrefix, CorInfoTailCall tailCallResult, const char* reason),
        callerHnd, calleeHnd, fIsTailPrefix, tailCallResult, reason
    );

    FORWARD_METHOD(
        void, getEHinfo,
        (CORINFO_METHOD_HANDLE ftn, unsigned EHnumber, CORINFO_EH_CLAUSE* clause),
        ftn, EHnumber, clause
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getMethodClass,
        (CORINFO_METHOD_HANDLE method),
        method
    );

    FORWARD_METHOD(
        void, getMethodVTableOffset,
        (CORINFO_METHOD_HANDLE method, unsigned* offsetOfIndirection,
        unsigned* offsetAfterIndirection, bool* isRelative),
        method, offsetOfIndirection, offsetAfterIndirection, isRelative
    );

    FORWARD_METHOD(
        bool, resolveVirtualMethod,
        (CORINFO_DEVIRTUALIZATION_INFO* info),
        info
    );

    FORWARD_METHOD(
        CORINFO_METHOD_HANDLE, getUnboxedEntry,
        (CORINFO_METHOD_HANDLE ftn, bool* requiresInstMethodTableArg),
        ftn, requiresInstMethodTableArg
    );

    FORWARD_METHOD(
        CORINFO_METHOD_HANDLE, getInstantiatedEntry,
        (CORINFO_METHOD_HANDLE ftn, CORINFO_METHOD_HANDLE* methodArg, CORINFO_CLASS_HANDLE* classArg),
        ftn, methodArg, classArg
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getDefaultComparerClass,
        (CORINFO_CLASS_HANDLE elemType),
        elemType
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getDefaultEqualityComparerClass,
        (CORINFO_CLASS_HANDLE elemType),
        elemType
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getSZArrayHelperEnumeratorClass,
        (CORINFO_CLASS_HANDLE elemType),
        elemType
    );

    FORWARD_METHOD(
        void, expandRawHandleIntrinsic,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, CORINFO_METHOD_HANDLE callerHandle,
        CORINFO_GENERICHANDLE_RESULT* pResult),
        pResolvedToken, callerHandle, pResult
    );

    FORWARD_METHOD(
        bool, isIntrinsicType,
        (CORINFO_CLASS_HANDLE classHnd),
        classHnd
    );

    FORWARD_METHOD(
        CorInfoCallConvExtension, getUnmanagedCallConv,
        (CORINFO_METHOD_HANDLE method, CORINFO_SIG_INFO* callSiteSig, bool* pSuppressGCTransition),
        method, callSiteSig, pSuppressGCTransition
    );

    FORWARD_METHOD(
        bool, pInvokeMarshalingRequired,
        (CORINFO_METHOD_HANDLE method, CORINFO_SIG_INFO* callSiteSig),
        method, callSiteSig
    );

    FORWARD_METHOD(
        bool, satisfiesMethodConstraints,
        (CORINFO_CLASS_HANDLE parent, CORINFO_METHOD_HANDLE method),
        parent, method
    );

    FORWARD_METHOD(
        void, methodMustBeLoadedBeforeCodeIsRun,
        (CORINFO_METHOD_HANDLE method),
        method
    );

    FORWARD_METHOD(
        void, getGSCookie,
        (GSCookie* pCookieVal, GSCookie** ppCookieVal),
        pCookieVal, ppCookieVal
    );

    FORWARD_METHOD(
        void, setPatchpointInfo,
        (PatchpointInfo* patchpointInfo),
        patchpointInfo
    );

    FORWARD_METHOD(
        PatchpointInfo*, getOSRInfo,
        (unsigned* ilOffset),
        ilOffset
    );

    FORWARD_METHOD(
        void, resolveToken,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken),
        pResolvedToken
    );

    FORWARD_METHOD(
        void, findSig,
        (CORINFO_MODULE_HANDLE module, unsigned sigTOK, CORINFO_CONTEXT_HANDLE context, CORINFO_SIG_INFO* sig),
        module, sigTOK, context, sig
    );

    FORWARD_METHOD(
        void, findCallSiteSig,
        (CORINFO_MODULE_HANDLE module, unsigned methTOK, CORINFO_CONTEXT_HANDLE context, CORINFO_SIG_INFO* sig),
        module, methTOK, context, sig
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getTokenTypeAsHandle,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken),
        pResolvedToken
    );

    FORWARD_METHOD(
        int, getStringLiteral,
        (CORINFO_MODULE_HANDLE module, unsigned metaTOK, char16_t* buffer, int bufferSize, int startIndex),
        module, metaTOK, buffer, bufferSize, startIndex
    );

    FORWARD_METHOD(
        size_t, printObjectDescription,
        (CORINFO_OBJECT_HANDLE handle, char* buffer, size_t bufferSize, size_t* pRequiredBufferSize),
        handle, buffer, bufferSize, pRequiredBufferSize
    );

    FORWARD_METHOD(
        CorInfoType, asCorInfoType,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        const char*, getClassNameFromMetadata,
        (CORINFO_CLASS_HANDLE cls, const char** namespaceName),
        cls, namespaceName
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getTypeInstantiationArgument,
        (CORINFO_CLASS_HANDLE cls, unsigned index),
        cls, index
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getMethodInstantiationArgument,
        (CORINFO_METHOD_HANDLE ftn, unsigned index),
        ftn, index
    );

    FORWARD_METHOD(
        size_t, printClassName,
        (CORINFO_CLASS_HANDLE cls, char* buffer, size_t bufferSize, size_t* pRequiredBufferSize),
        cls, buffer, bufferSize, pRequiredBufferSize
    );

    FORWARD_METHOD(
        bool, isValueClass,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        uint32_t, getClassAttribs,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        const char*, getClassAssemblyName,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        void*, LongLifetimeMalloc,
        (size_t sz),
        sz
    );

    FORWARD_METHOD(
        void, LongLifetimeFree,
        (void* obj),
        obj
    );

    FORWARD_METHOD(
        bool, getIsClassInitedFlagAddress,
        (CORINFO_CLASS_HANDLE cls, CORINFO_CONST_LOOKUP* addr, int* offset),
        cls, addr, offset
    );

    FORWARD_METHOD(
        size_t, getClassStaticDynamicInfo,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        size_t, getClassThreadStaticDynamicInfo,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        bool, getStaticBaseAddress,
        (CORINFO_CLASS_HANDLE cls, bool isGc, CORINFO_CONST_LOOKUP* addr),
        cls, isGc, addr
    );

    FORWARD_METHOD(
        unsigned, getClassSize,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        unsigned, getHeapClassSize,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        bool, canAllocateOnStack,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        unsigned, getClassAlignmentRequirement,
        (CORINFO_CLASS_HANDLE cls, bool fDoubleAlignHint),
        cls, fDoubleAlignHint
    );

    FORWARD_METHOD(
        unsigned, getClassGClayout,
        (CORINFO_CLASS_HANDLE cls, uint8_t* gcPtrs),
        cls, gcPtrs
    );

    FORWARD_METHOD(
        unsigned, getClassNumInstanceFields,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CORINFO_FIELD_HANDLE, getFieldInClass,
        (CORINFO_CLASS_HANDLE clsHnd, int32_t num),
        clsHnd, num
    );

    FORWARD_METHOD(
        GetTypeLayoutResult, getTypeLayout,
        (CORINFO_CLASS_HANDLE typeHnd, CORINFO_TYPE_LAYOUT_NODE* treeNodes, size_t* numTreeNodes),
        typeHnd, treeNodes, numTreeNodes
    );

    FORWARD_METHOD(
        bool, checkMethodModifier,
        (CORINFO_METHOD_HANDLE hMethod, const char* modifier, bool fOptional),
        hMethod, modifier, fOptional
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getNewHelper,
        (CORINFO_CLASS_HANDLE classHandle, bool* pHasSideEffects),
        classHandle, pHasSideEffects
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getNewArrHelper,
        (CORINFO_CLASS_HANDLE arrayCls),
        arrayCls
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getCastingHelper,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, bool fThrowing),
        pResolvedToken, fThrowing
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getSharedCCtorHelper,
        (CORINFO_CLASS_HANDLE clsHnd),
        clsHnd
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getTypeForBox,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getTypeForBoxOnStack,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getBoxHelper,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CorInfoHelpFunc, getUnBoxHelper,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CORINFO_OBJECT_HANDLE, getRuntimeTypePointer,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        bool, isObjectImmutable,
        (CORINFO_OBJECT_HANDLE objPtr),
        objPtr
    );

    FORWARD_METHOD(
        bool, getStringChar,
        (CORINFO_OBJECT_HANDLE strObj, int index, uint16_t* value),
        strObj, index, value
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getObjectType,
        (CORINFO_OBJECT_HANDLE objPtr),
        objPtr
    );

    FORWARD_METHOD(
        bool, getReadyToRunHelper,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, CORINFO_LOOKUP_KIND* pGenericLookupKind,
        CorInfoHelpFunc id, CORINFO_METHOD_HANDLE callerHandle, CORINFO_CONST_LOOKUP* pLookup),
        pResolvedToken, pGenericLookupKind, id, callerHandle, pLookup
    );

    FORWARD_METHOD(
        void, getReadyToRunDelegateCtorHelper,
        (CORINFO_RESOLVED_TOKEN* pTargetMethod, mdToken targetConstraint, CORINFO_CLASS_HANDLE delegateType,
        CORINFO_METHOD_HANDLE callerHandle, CORINFO_LOOKUP* pLookup),
        pTargetMethod, targetConstraint, delegateType, callerHandle, pLookup
    );

    FORWARD_METHOD(
        CorInfoInitClassResult, initClass,
        (CORINFO_FIELD_HANDLE field, CORINFO_METHOD_HANDLE method, CORINFO_CONTEXT_HANDLE context),
        field, method, context
    );

    FORWARD_METHOD(
        void, classMustBeLoadedBeforeCodeIsRun,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getBuiltinClass,
        (CorInfoClassId classId),
        classId
    );

    FORWARD_METHOD(
        CorInfoType, getTypeForPrimitiveValueClass,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CorInfoType, getTypeForPrimitiveNumericClass,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        bool, canCast,
        (CORINFO_CLASS_HANDLE child, CORINFO_CLASS_HANDLE parent),
        child, parent
    );

    FORWARD_METHOD(
        TypeCompareState, compareTypesForCast,
        (CORINFO_CLASS_HANDLE fromClass, CORINFO_CLASS_HANDLE toClass),
        fromClass, toClass
    );

    FORWARD_METHOD(
        TypeCompareState, compareTypesForEquality,
        (CORINFO_CLASS_HANDLE cls1, CORINFO_CLASS_HANDLE cls2),
        cls1, cls2
    );

    FORWARD_METHOD(
        bool, isMoreSpecificType,
        (CORINFO_CLASS_HANDLE cls1, CORINFO_CLASS_HANDLE cls2),
        cls1, cls2
    );

    FORWARD_METHOD(
        bool, isExactType,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        TypeCompareState, isGenericType,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        TypeCompareState, isNullableType,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        TypeCompareState, isEnum,
        (CORINFO_CLASS_HANDLE cls, CORINFO_CLASS_HANDLE* underlyingType),
        cls, underlyingType
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getParentType,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CorInfoType, getChildType,
        (CORINFO_CLASS_HANDLE clsHnd, CORINFO_CLASS_HANDLE* clsRet),
        clsHnd, clsRet
    );

    FORWARD_METHOD(
        bool, isSDArray,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        unsigned, getArrayRank,
        (CORINFO_CLASS_HANDLE cls),
        cls
    );

    FORWARD_METHOD(
        CorInfoArrayIntrinsic, getArrayIntrinsicID,
        (CORINFO_METHOD_HANDLE ftn),
        ftn
    );

    FORWARD_METHOD(
        void*, getArrayInitializationData,
        (CORINFO_FIELD_HANDLE field, uint32_t size),
        field, size
    );

    FORWARD_METHOD(
        CorInfoIsAccessAllowedResult, canAccessClass,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, CORINFO_METHOD_HANDLE callerHandle, CORINFO_HELPER_DESC* pAccessHelper),
        pResolvedToken, callerHandle, pAccessHelper
    );

    FORWARD_METHOD(
        size_t, printFieldName,
        (CORINFO_FIELD_HANDLE field, char* buffer, size_t bufferSize, size_t* pRequiredBufferSize),
        field, buffer, bufferSize, pRequiredBufferSize
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getFieldClass,
        (CORINFO_FIELD_HANDLE field),
        field
    );

    FORWARD_METHOD(
        CorInfoType, getFieldType,
        (CORINFO_FIELD_HANDLE field, CORINFO_CLASS_HANDLE* structType, CORINFO_CLASS_HANDLE fieldOwnerHint),
        field, structType, fieldOwnerHint
    );

    FORWARD_METHOD(
        unsigned, getFieldOffset,
        (CORINFO_FIELD_HANDLE field),
        field
    );

    FORWARD_METHOD(
        void, getFieldInfo,
        (CORINFO_RESOLVED_TOKEN* pResolvedToken, CORINFO_METHOD_HANDLE callerHandle,
        CORINFO_ACCESS_FLAGS flags, CORINFO_FIELD_INFO* pResult),
        pResolvedToken, callerHandle, flags, pResult
    );

    FORWARD_METHOD(
        uint32_t, getThreadLocalFieldInfo,
        (CORINFO_FIELD_HANDLE field, bool isGCType),
        field, isGCType
    );

    FORWARD_METHOD(
        void, getThreadLocalStaticBlocksInfo,
        (CORINFO_THREAD_STATIC_BLOCKS_INFO* pInfo),
        pInfo
    );

    FORWARD_METHOD(
        void, getThreadLocalStaticInfo_NativeAOT,
        (CORINFO_THREAD_STATIC_INFO_NATIVEAOT* pInfo),
        pInfo
    );

    FORWARD_METHOD(
        bool, isFieldStatic,
        (CORINFO_FIELD_HANDLE fldHnd),
        fldHnd
    );

    FORWARD_METHOD(
        int, getArrayOrStringLength,
        (CORINFO_OBJECT_HANDLE objHnd),
        objHnd
    );

    FORWARD_METHOD(
        void, getBoundaries,
        (CORINFO_METHOD_HANDLE ftn, unsigned int* cILOffsets, uint32_t** pILOffsets,
        ICorDebugInfo::BoundaryTypes* implicitBoundaries),
        ftn, cILOffsets, pILOffsets, implicitBoundaries
    );

    FORWARD_METHOD(
        void, setBoundaries,
        (CORINFO_METHOD_HANDLE ftn, uint32_t cMap, ICorDebugInfo::OffsetMapping* pMap),
        ftn, cMap, pMap
    );

    FORWARD_METHOD(
        void, getVars,
        (CORINFO_METHOD_HANDLE ftn, uint32_t* cVars, ICorDebugInfo::ILVarInfo** vars, bool* extendOthers),
        ftn, cVars, vars, extendOthers
    );

    FORWARD_METHOD(
        void, setVars,
        (CORINFO_METHOD_HANDLE ftn, uint32_t cVars, ICorDebugInfo::NativeVarInfo* vars),
        ftn, cVars, vars
    );

    FORWARD_METHOD(
        void, reportRichMappings,
        (ICorDebugInfo::InlineTreeNode* inlineTreeNodes, uint32_t numInlineTreeNodes,
        ICorDebugInfo::RichOffsetMapping* mappings, uint32_t numMappings),
        inlineTreeNodes, numInlineTreeNodes, mappings, numMappings
    );

    FORWARD_METHOD(
        void, reportMetadata,
        (const char* key, const void* value, size_t length),
        key, value, length
    );

    FORWARD_METHOD(
        void*, allocateArray,
        (size_t cBytes),
        cBytes
    );

    FORWARD_METHOD(
        void, freeArray,
        (void* array),
        array
    );

    FORWARD_METHOD(
        CORINFO_ARG_LIST_HANDLE, getArgNext,
        (CORINFO_ARG_LIST_HANDLE args),
        args
    );

    FORWARD_METHOD(
        CorInfoTypeWithMod, getArgType,
        (CORINFO_SIG_INFO* sig, CORINFO_ARG_LIST_HANDLE args, CORINFO_CLASS_HANDLE* vcTypeRet),
        sig, args, vcTypeRet
    );

    FORWARD_METHOD(
        int, getExactClasses,
        (CORINFO_CLASS_HANDLE baseType, int maxExactClasses, CORINFO_CLASS_HANDLE* exactClsRet),
        baseType, maxExactClasses, exactClsRet
    );

    FORWARD_METHOD(
        CORINFO_CLASS_HANDLE, getArgClass,
        (CORINFO_SIG_INFO* sig, CORINFO_ARG_LIST_HANDLE args),
        sig, args
    );

    FORWARD_METHOD(
        CorInfoHFAElemType, getHFAType,
        (CORINFO_CLASS_HANDLE hClass),
        hClass
    );

    FORWARD_METHOD(
        bool, runWithErrorTrap,
        (errorTrapFunction function, void* parameter),
        function, parameter
    );

    FORWARD_METHOD(
        bool, runWithSPMIErrorTrap,
        (errorTrapFunction function, void* parameter),
        function, parameter
    );

    FORWARD_METHOD(
        void, getEEInfo,
        (CORINFO_EE_INFO* pEEInfoOut),
        pEEInfoOut
    );

    FORWARD_METHOD(
        mdMethodDef, getMethodDefFromMethod,
        (CORINFO_METHOD_HANDLE hMethod),
        hMethod
    );

    FORWARD_METHOD(
        size_t, printMethodName,
        (CORINFO_METHOD_HANDLE ftn, char* buffer, size_t bufferSize, size_t* pRequiredBufferSize),
        ftn, buffer, bufferSize, pRequiredBufferSize
    );

    FORWARD_METHOD(
        const char*, getMethodNameFromMetadata,
        (CORINFO_METHOD_HANDLE ftn, const char** className, const char** namespaceName,
        const char** enclosingClassNames, size_t maxEnclosingClassNames),
        ftn, className, namespaceName, enclosingClassNames, maxEnclosingClassNames
    );

    FORWARD_METHOD(
        unsigned, getMethodHash,
        (CORINFO_METHOD_HANDLE ftn),
        ftn
    );

    FORWARD_METHOD(
        bool, getSystemVAmd64PassStructInRegisterDescriptor,
        (CORINFO_CLASS_HANDLE structHnd, SYSTEMV_AMD64_CORINFO_STRUCT_REG_PASSING_DESCRIPTOR* structPassInRegDescPtr),
        structHnd, structPassInRegDescPtr
    );

    FORWARD_METHOD(
        void, getSwiftLowering,
        (CORINFO_CLASS_HANDLE structHnd, CORINFO_SWIFT_LOWERING* pLowering),
        structHnd, pLowering
    );

    FORWARD_METHOD(
        void, getFpStructLowering,
        (CORINFO_CLASS_HANDLE structHnd, CORINFO_FPSTRUCT_LOWERING* pLowering),
        structHnd, pLowering
    );
};
