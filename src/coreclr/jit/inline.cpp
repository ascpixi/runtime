// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include "jitpch.h"
#ifdef _MSC_VER
#pragma hdrstop
#endif

#include "inlinepolicy.h"

// Lookup table for inline description strings

static const char* InlineDescriptions[] = {
#define INLINE_OBSERVATION(name, type, description, impact, target) description,
#include "inline.def"
#undef INLINE_OBSERVATION
};

// Lookup table for inline targets

static const InlineTarget InlineTargets[] = {
#define INLINE_OBSERVATION(name, type, description, impact, target) InlineTarget::target,
#include "inline.def"
#undef INLINE_OBSERVATION
};

// Lookup table for inline impacts

static const InlineImpact InlineImpacts[] = {
#define INLINE_OBSERVATION(name, type, description, impact, target) InlineImpact::impact,
#include "inline.def"
#undef INLINE_OBSERVATION
};

#ifdef DEBUG

//------------------------------------------------------------------------
// InlIsValidObservation: run a validity check on an inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    true if the observation is valid

bool InlIsValidObservation(InlineObservation obs)
{
    return ((obs > InlineObservation::CALLEE_UNUSED_INITIAL) && (obs < InlineObservation::CALLEE_UNUSED_FINAL));
}

#endif // DEBUG

//------------------------------------------------------------------------
// InlGetObservationString: get a string describing this inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    string describing the observation

const char* InlGetObservationString(InlineObservation obs)
{
    assert(InlIsValidObservation(obs));
    return InlineDescriptions[static_cast<int>(obs)];
}

//------------------------------------------------------------------------
// InlGetTarget: get the target of an inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    enum describing the target

InlineTarget InlGetTarget(InlineObservation obs)
{
    assert(InlIsValidObservation(obs));
    return InlineTargets[static_cast<int>(obs)];
}

//------------------------------------------------------------------------
// InlGetTargetString: get a string describing the target of an inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    string describing the target

const char* InlGetTargetString(InlineObservation obs)
{
    InlineTarget t = InlGetTarget(obs);
    switch (t)
    {
        case InlineTarget::CALLER:
            return "caller";
        case InlineTarget::CALLEE:
            return "callee";
        case InlineTarget::CALLSITE:
            return "call site";
        default:
            return "unexpected target";
    }
}

//------------------------------------------------------------------------
// InlGetImpact: get the impact of an inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    enum value describing the impact

InlineImpact InlGetImpact(InlineObservation obs)
{
    assert(InlIsValidObservation(obs));
    return InlineImpacts[static_cast<int>(obs)];
}

//------------------------------------------------------------------------
// InlGetImpactString: get a string describing the impact of an inline observation
//
// Arguments:
//    obs - the observation in question
//
// Return Value:
//    string describing the impact

const char* InlGetImpactString(InlineObservation obs)
{
    InlineImpact i = InlGetImpact(obs);
    switch (i)
    {
        case InlineImpact::FATAL:
            return "correctness -- fatal";
        case InlineImpact::FUNDAMENTAL:
            return "correctness -- fundamental limitation";
        case InlineImpact::LIMITATION:
            return "correctness -- jit limitation";
        case InlineImpact::PERFORMANCE:
            return "performance";
        case InlineImpact::INFORMATION:
            return "information";
        default:
            return "unexpected impact";
    }
}

//------------------------------------------------------------------------
// InlGetCorInfoInlineDecision: translate decision into a CorInfoInline
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    CorInfoInline value representing the decision

CorInfoInline InlGetCorInfoInlineDecision(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::SUCCESS:
            return INLINE_PASS;
        case InlineDecision::FAILURE:
            return INLINE_FAIL;
        case InlineDecision::NEVER:
            return INLINE_NEVER;
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlGetDecisionString: get a string representing this decision
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    string representing the decision

const char* InlGetDecisionString(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::SUCCESS:
            return "success";
        case InlineDecision::FAILURE:
            return "failed this call site";
        case InlineDecision::NEVER:
            return "failed this callee";
        case InlineDecision::CANDIDATE:
            return "candidate";
        case InlineDecision::UNDECIDED:
            return "undecided";
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlDecisionIsFailure: check if this decision describes a failing inline
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    true if the inline is definitely a failure

bool InlDecisionIsFailure(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::SUCCESS:
        case InlineDecision::UNDECIDED:
        case InlineDecision::CANDIDATE:
            return false;
        case InlineDecision::FAILURE:
        case InlineDecision::NEVER:
            return true;
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlDecisionIsSuccess: check if this decision describes a successful inline
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    true if the inline is definitely a success

bool InlDecisionIsSuccess(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::SUCCESS:
            return true;
        case InlineDecision::FAILURE:
        case InlineDecision::NEVER:
        case InlineDecision::UNDECIDED:
        case InlineDecision::CANDIDATE:
            return false;
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlDecisionIsNever: check if this decision describes a never inline
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    true if the inline is a never inline case

bool InlDecisionIsNever(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::NEVER:
            return true;
        case InlineDecision::FAILURE:
        case InlineDecision::SUCCESS:
        case InlineDecision::UNDECIDED:
        case InlineDecision::CANDIDATE:
            return false;
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlDecisionIsCandidate: check if this decision describes a viable candidate
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    true if this inline still might happen

bool InlDecisionIsCandidate(InlineDecision d)
{
    return !InlDecisionIsFailure(d);
}

//------------------------------------------------------------------------
// InlDecisionIsDecided: check if this decision has been made
//
// Arguments:
//    d - the decision in question
//
// Return Value:
//    true if this inline has been decided one way or another

bool InlDecisionIsDecided(InlineDecision d)
{
    switch (d)
    {
        case InlineDecision::NEVER:
        case InlineDecision::FAILURE:
        case InlineDecision::SUCCESS:
            return true;
        case InlineDecision::UNDECIDED:
        case InlineDecision::CANDIDATE:
            return false;
        default:
            assert(!"Unexpected InlineDecision");
            unreached();
    }
}

//------------------------------------------------------------------------
// InlineContext: default constructor

InlineContext::InlineContext(InlineStrategy* strategy)
    : m_InlineStrategy(strategy)
    , m_Parent(nullptr)
    , m_Child(nullptr)
    , m_Sibling(nullptr)
    , m_Code(nullptr)
    , m_Callee(nullptr)
    , m_RuntimeContext(nullptr)
    , m_ILSize(0)
    , m_ImportedILSize(0)
    , m_ActualCallOffset(BAD_IL_OFFSET)
    , m_Observation(InlineObservation::CALLEE_UNUSED_INITIAL)
    , m_CodeSizeEstimate(0)
    , m_Ordinal(0)
    , m_Success(true)
#if defined(DEBUG)
    , m_Policy(nullptr)
    , m_TreeID(0)
    , m_Devirtualized(false)
    , m_Guarded(false)
    , m_Unboxed(false)
    , m_ILInstsSet(nullptr)
#endif
{
    // Empty
}

//------------------------------------------------------------------------
// InlineResult: Construct an InlineResult to evaluate a particular call
// for inlining.
//
// Arguments:
//   compiler      - the compiler instance examining a call for inlining
//   call          - the call in question
//   stmt          - statement containing the call (if known)
//   description   - string describing the context of the decision

InlineResult::InlineResult(
    Compiler* compiler, GenTreeCall* call, Statement* stmt, const char* description, bool doNotReport)
    : m_RootCompiler(nullptr)
    , m_Policy(nullptr)
    , m_Call(call)
    , m_InlineContext(nullptr)
    , m_Caller(nullptr)
    , m_Callee(nullptr)
    , m_ImportedILSize(0)
    , m_Description(description)
    , m_successResult(INLINE_PASS)
    , m_DoNotReport(doNotReport)
    , m_reportFailureAsVmFailure(false)
{
    // Set the compiler instance
    m_RootCompiler = compiler->impInlineRoot();

    // Set the policy
    const bool isPrejitRoot = false;
    m_Policy                = InlinePolicy::GetPolicy(m_RootCompiler, isPrejitRoot);

    // Pass along some optional information to the policy.
    if (stmt != nullptr)
    {
        m_InlineContext = stmt->GetDebugInfo().GetInlineContext();
        m_Policy->NoteContext(m_InlineContext);

#if defined(DEBUG)
        m_Policy->NoteOffset(call->gtRawILOffset);
#else
        m_Policy->NoteOffset(stmt->GetDebugInfo().GetLocation().GetOffset());
#endif // defined(DEBUG)
    }

    // Get method handle for caller. Note we use the
    // handle for the "immediate" caller here.
    m_Caller = compiler->info.compMethodHnd;

    // Get method handle for callee, if known
    if (m_Call->AsCall()->gtCallType == CT_USER_FUNC)
    {
        m_Callee = m_Call->AsCall()->gtCallMethHnd;
    }

    if (!m_DoNotReport)
    {
        COMP_HANDLE comp = m_RootCompiler->info.compCompHnd;
        comp->beginInlining(m_Caller, m_Callee);
    }
}

//------------------------------------------------------------------------
// InlineResult: Construct an InlineResult to evaluate a particular
// method as a possible inline candidate, while prejtting.
//
// Arguments:
//    compiler    - the compiler instance doing the prejitting
//    method      - the method in question
//    description - string describing the context of the decision
//
// Notes:
//    Used only during prejitting to try and pre-identify methods that
//    cannot be inlined, to help subsequent jit throughput.
//
//    We use the inlCallee member to track the method since logically
//    it is the callee here.

InlineResult::InlineResult(Compiler* compiler, CORINFO_METHOD_HANDLE method, const char* description, bool doNotReport)
    : m_RootCompiler(nullptr)
    , m_Policy(nullptr)
    , m_Call(nullptr)
    , m_InlineContext(nullptr)
    , m_Caller(nullptr)
    , m_Callee(method)
    , m_Description(description)
    , m_successResult(INLINE_PASS)
    , m_DoNotReport(doNotReport)
    , m_reportFailureAsVmFailure(false)
{
    // Set the compiler instance
    m_RootCompiler = compiler->impInlineRoot();

    // Set the policy
    const bool isPrejitRoot = true;
    m_Policy                = InlinePolicy::GetPolicy(m_RootCompiler, isPrejitRoot);

    if (!m_DoNotReport)
    {
        COMP_HANDLE comp = m_RootCompiler->info.compCompHnd;
        comp->beginInlining(m_Caller, m_Callee);
    }
}

//------------------------------------------------------------------------
// Report: Dump, log, and report information about an inline decision.
//
// Notes:
//    Called (automatically via the InlineResult dtor) when the
//    inliner is done evaluating a candidate.
//
//    Dumps state of the inline candidate, and if a decision was
//    reached, sends it to the log and reports the decision back to the
//    EE. Optionally update the method attribute to NOINLINE if
//    observation and policy warrant.
//

void InlineResult::Report()
{

#ifdef DEBUG
    // If this is a failure of a specific inline candidate and we haven't captured
    // a failing observation yet, do so now.
    if (IsFailure() && (m_Call != nullptr))
    {
        // compiler should have revoked candidacy on the call by now
        // Unless it's a call has both failed and successful candidates (GDV candidates)
        assert(((m_Call->gtFlags & GTF_CALL_INLINE_CANDIDATE) == 0) || m_Call->IsGuardedDevirtualizationCandidate());

        if (m_Call->gtInlineObservation == InlineObservation::CALLEE_UNUSED_INITIAL)
        {
            m_Call->gtInlineObservation = m_Policy->GetObservation();
        }
    }
#endif // DEBUG

    // If we weren't actually inlining, user may have suppressed
    // reporting. If so, do nothing.
    if (m_DoNotReport)
    {
        return;
    }

    m_DoNotReport = true;

#ifdef DEBUG
    const char* callee = nullptr;

    // Optionally dump the result
    if (VERBOSE || m_RootCompiler->fgPrintInlinedMethods)
    {
        const char* format = "INLINER: during '%s' result '%s' reason '%s' for '%s' calling '%s'\n";
        const char* caller = (m_Caller == nullptr) ? "n/a" : m_RootCompiler->eeGetMethodFullName(m_Caller);

        callee = (m_Callee == nullptr) ? "n/a" : m_RootCompiler->eeGetMethodFullName(m_Callee);

        JITDUMP(format, m_Description, ResultString(), ReasonString(), caller, callee);
    }
#endif // DEBUG

    // Was the result NEVER? If so we might want to propagate this to
    // the runtime.

    if (IsNever() && m_Policy->PropagateNeverToRuntime())
    {
        // If we know the callee, and if the observation that got us
        // to this Never inline state is something *other* than
        // IS_NOINLINE, then we've uncovered a reason why this method
        // can't ever be inlined. Update the callee method attributes
        // so that future inline attempts for this callee fail faster.

        InlineObservation obs = m_Policy->GetObservation();

        if ((m_Callee != nullptr) && (obs != InlineObservation::CALLEE_IS_NOINLINE))
        {
            JITDUMP("\nINLINER: Marking %s as NOINLINE because of %s\n", callee, InlGetObservationString(obs));

            COMP_HANDLE comp = m_RootCompiler->info.compCompHnd;
            comp->setMethodAttribs(m_Callee, CORINFO_FLG_BAD_INLINEE);
        }
    }

    if (IsDecided() || m_reportFailureAsVmFailure || m_successResult != INLINE_PASS)
    {
        const char* format = "INLINER: during '%s' result '%s' reason '%s'\n";
        JITLOG_THIS(m_RootCompiler, (LL_INFO100000, format, m_Description, ResultString(), ReasonString()));
        COMP_HANDLE comp = m_RootCompiler->info.compCompHnd;
        comp->reportInliningDecision(m_Caller, m_Callee, Result(), ReasonString());
    }
}

//------------------------------------------------------------------------
// InlineStrategy constructor
//
// Arguments
//    compiler - root compiler instance

InlineStrategy::InlineStrategy(Compiler* compiler)
    : m_Compiler(compiler)
    , m_RootContext(nullptr)
    , m_LastSuccessfulPolicy(nullptr)
    , m_LastContext(nullptr)
    , m_PrejitRootDecision(InlineDecision::UNDECIDED)
    , m_CallCount(0)
    , m_CandidateCount(0)
    , m_AlwaysCandidateCount(0)
    , m_ForceCandidateCount(0)
    , m_DiscretionaryCandidateCount(0)
    , m_UnprofitableCandidateCount(0)
    , m_ImportCount(0)
    , m_InlineCount(0)
    , m_MaxInlineSize(DEFAULT_MAX_INLINE_SIZE)
    , m_MaxInlineDepth(DEFAULT_MAX_INLINE_DEPTH)
    , m_MaxForceInlineDepth(DEFAULT_MAX_FORCE_INLINE_DEPTH)
    , m_InitialTimeBudget(0)
    , m_InitialTimeEstimate(0)
    , m_CurrentTimeBudget(0)
    , m_CurrentTimeEstimate(0)
    , m_InitialSizeEstimate(0)
    , m_CurrentSizeEstimate(0)
    , m_HasForceViaDiscretionary(false)
#if defined(DEBUG)
    , m_MethodXmlFilePosition(0)
    , m_Random(nullptr)
#endif // defined(DEBUG)

{
    // Verify compiler is a root compiler instance
    assert(m_Compiler->impInlineRoot() == m_Compiler);

#ifdef DEBUG

    // Possibly modify the max inline size.
    //
    // Default value of JitInlineSize is the same as our default.
    // So normally this next line does not change the size.
    m_MaxInlineSize = JitConfig.JitInlineSize();

    // Up the max size under stress
    if (m_Compiler->compInlineStress())
    {
        m_MaxInlineSize *= 10;
    }

    // But don't overdo it
    if (m_MaxInlineSize > IMPLEMENTATION_MAX_INLINE_SIZE)
    {
        m_MaxInlineSize = IMPLEMENTATION_MAX_INLINE_SIZE;
    }

    // Verify: not too small, not too big.
    assert(m_MaxInlineSize >= ALWAYS_INLINE_SIZE);
    assert(m_MaxInlineSize <= IMPLEMENTATION_MAX_INLINE_SIZE);

    // Possibly modify the max inline depth
    //
    // Default value of JitInlineDepth is the same as our default.
    // So normally this next line does not change the size.
    m_MaxInlineDepth = JitConfig.JitInlineDepth();

    // But don't overdo it
    if (m_MaxInlineDepth > IMPLEMENTATION_MAX_INLINE_DEPTH)
    {
        m_MaxInlineDepth = IMPLEMENTATION_MAX_INLINE_DEPTH;
    }

    // Possibly modify the max force inline depth
    //
    // Default value of JitForceInlineDepth is the same as our default.
    // So normally this next line does not change the size.
    m_MaxForceInlineDepth = JitConfig.JitForceInlineDepth();

    // But don't overdo it
    if (m_MaxForceInlineDepth > m_MaxInlineDepth)
    {
        m_MaxForceInlineDepth = m_MaxInlineDepth;
    }

#endif // DEBUG
}

//------------------------------------------------------------------------
// GetRootContext: get the InlineContext for the root method
//
// Return Value:
//    Root context; describes the method being jitted.
//
// Note:
//    Also initializes the jit time estimate and budget.

InlineContext* InlineStrategy::GetRootContext()
{
    if (m_RootContext == nullptr)
    {
        // Allocate on first demand.
        m_RootContext = NewRoot();

        // Estimate how long the jit will take if there's no inlining
        // done to this method.
        m_InitialTimeEstimate = EstimateTime(m_RootContext);
        m_CurrentTimeEstimate = m_InitialTimeEstimate;

        // Set the initial budget for inlining. Note this is
        // deliberately set very high and is intended to catch
        // only pathological runaway inline cases.
        m_InitialTimeBudget = BUDGET * m_InitialTimeEstimate;
        m_CurrentTimeBudget = m_InitialTimeBudget;

        // Estimate the code size  if there's no inlining
        m_InitialSizeEstimate = EstimateSize(m_RootContext);
        m_CurrentSizeEstimate = m_InitialSizeEstimate;

        // Sanity check
        assert(m_CurrentTimeEstimate > 0);
        assert(m_CurrentSizeEstimate > 0);

        // Cache as the "last" context created
        m_LastContext = m_RootContext;
    }

    return m_RootContext;
}

//------------------------------------------------------------------------
// NoteAttempt: do bookkeeping for an inline attempt
//
// Arguments:
//    result -- InlineResult for successful inline candidate

void InlineStrategy::NoteAttempt(InlineResult* result)
{
    assert(result->IsCandidate());
    InlineObservation obs = result->GetObservation();

    if (obs == InlineObservation::CALLEE_BELOW_ALWAYS_INLINE_SIZE)
    {
        m_AlwaysCandidateCount++;
    }
    else if (obs == InlineObservation::CALLEE_IS_FORCE_INLINE)
    {
        m_ForceCandidateCount++;
    }
    else
    {
        m_DiscretionaryCandidateCount++;
    }
}

//------------------------------------------------------------------------
// DumpCsvHeader: dump header for csv inline stats
//
// Argument:
//     fp -- file for dump output

void InlineStrategy::DumpCsvHeader(FILE* fp)
{
    fprintf(fp, "\"InlineCalls\",");
    fprintf(fp, "\"InlineCandidates\",");
    fprintf(fp, "\"InlineAlways\",");
    fprintf(fp, "\"InlineForce\",");
    fprintf(fp, "\"InlineDiscretionary\",");
    fprintf(fp, "\"InlineUnprofitable\",");
    fprintf(fp, "\"InlineEarlyFail\",");
    fprintf(fp, "\"InlineImport\",");
    fprintf(fp, "\"InlineLateFail\",");
    fprintf(fp, "\"InlineSuccess\",");
}

//------------------------------------------------------------------------
// DumpCsvData: dump data for csv inline stats
//
// Argument:
//     fp -- file for dump output

void InlineStrategy::DumpCsvData(FILE* fp)
{
    fprintf(fp, "%u,", m_CallCount);
    fprintf(fp, "%u,", m_CandidateCount);
    fprintf(fp, "%u,", m_AlwaysCandidateCount);
    fprintf(fp, "%u,", m_ForceCandidateCount);
    fprintf(fp, "%u,", m_DiscretionaryCandidateCount);
    fprintf(fp, "%u,", m_UnprofitableCandidateCount);

    // Early failures are cases where candates are rejected between
    // the time the jit invokes the inlinee compiler and the time it
    // starts to import the inlinee IL.
    //
    // So they are "cheaper" that late failures.

    unsigned profitableCandidateCount = m_DiscretionaryCandidateCount - m_UnprofitableCandidateCount;

    unsigned earlyFailCount =
        m_CandidateCount - m_AlwaysCandidateCount - m_ForceCandidateCount - profitableCandidateCount;

    fprintf(fp, "%u,", earlyFailCount);

    unsigned lateFailCount = m_ImportCount - m_InlineCount;

    fprintf(fp, "%u,", m_ImportCount);
    fprintf(fp, "%u,", lateFailCount);
    fprintf(fp, "%u,", m_InlineCount);
}

//------------------------------------------------------------------------
// EstimateTime: estimate impact of this inline on the method jit time
//
// Arguments:
//     context - context describing this inline
//
// Return Value:
//    Nominal estimate of jit time.

int InlineStrategy::EstimateTime(InlineContext* context)
{
    // Simple linear models based on observations
    // show time is fairly well predicted by IL size.
    //
    // Prediction varies for root and inlines.
    if (context == m_RootContext)
    {
        return EstimateRootTime(context->GetILSize());
    }
    else
    {
        // Use amount of IL actually imported
        return EstimateInlineTime(context->GetImportedILSize());
    }
}

//------------------------------------------------------------------------
// EstimteRootTime: estimate jit time for method of this size with
// no inlining.
//
// Arguments:
//    ilSize - size of the method's IL
//
// Return Value:
//    Nominal estimate of jit time.
//
// Notes:
//    Based on observational data. Time is nominally microseconds.

int InlineStrategy::EstimateRootTime(unsigned ilSize)
{
    return 60 + 3 * ilSize;
}

//------------------------------------------------------------------------
// EstimteInlineTime: estimate time impact on jitting for an inline
// of this size.
//
// Arguments:
//    ilSize - size of the method's IL
//
// Return Value:
//    Nominal increase in jit time.
//
// Notes:
//    Based on observational data. Time is nominally microseconds.
//    Small inlines will make the jit a bit faster.

int InlineStrategy::EstimateInlineTime(unsigned ilSize)
{
    return -14 + 2 * ilSize;
}

//------------------------------------------------------------------------
// EstimateSize: estimate impact of this inline on the method size
//
// Arguments:
//     context - context describing this inline
//
// Return Value:
//    Nominal estimate of method size (bytes * 10)

int InlineStrategy::EstimateSize(InlineContext* context)
{
    // Prediction varies for root and inlines.
    if (context == m_RootContext)
    {
        // Simple linear models based on observations show root method
        // native code size is fairly well predicted by IL size.
        //
        // Model below is for x64 on windows.
        unsigned ilSize   = context->GetILSize();
        int      estimate = (1312 + 228 * ilSize) / 10;

        return estimate;
    }
    else
    {
        // Use context's code size estimate.
        return context->GetCodeSizeEstimate();
    }
}

//------------------------------------------------------------------------
// NoteOutcome: do bookkeeping for an inline
//
// Arguments:
//    context - context for the inlie

void InlineStrategy::NoteOutcome(InlineContext* context)
{
    // Note we can't generally count up failures here -- we only
    // create contexts for failures in debug modes, and even then
    // we may not get them all.
    if (context->IsSuccess())
    {
        m_InlineCount++;

#if defined(DEBUG)

        // Keep track of the inline targeted for data collection or,
        // if we don't have one (yet), the last successful inline.
        bool updateLast = (m_LastSuccessfulPolicy == nullptr) || !m_LastSuccessfulPolicy->IsDataCollectionTarget();

        if (updateLast)
        {
            m_LastContext          = context;
            m_LastSuccessfulPolicy = context->m_Policy;
        }
        else
        {
            // We only expect one inline to be a data collection
            // target.
            assert(!context->m_Policy->IsDataCollectionTarget());
        }

#endif // defined(DEBUG)

        // Budget update.
        //
        // If callee is a force inline, increase budget, provided all
        // parent contexts are likewise force inlines.
        //
        // If callee is discretionary or has a discretionary ancestor,
        // increase expense.

        InlineContext* currentContext = context;
        bool           isForceInline  = false;

        while (currentContext != m_RootContext)
        {
            InlineObservation observation = currentContext->GetObservation();

            if (observation != InlineObservation::CALLEE_IS_FORCE_INLINE)
            {
                if (isForceInline)
                {
                    // Interesting case where discretionary inlines pull
                    // in a force inline...
                    m_HasForceViaDiscretionary = true;
                }

                isForceInline = false;
                break;
            }

            isForceInline  = true;
            currentContext = currentContext->GetParent();
        }

        int timeDelta = EstimateTime(context);

        if (isForceInline)
        {
            // Update budget since this inline was forced.  Only allow
            // budget to increase.
            if (timeDelta > 0)
            {
                m_CurrentTimeBudget += timeDelta;
            }
        }

        // Update time estimate.
        m_CurrentTimeEstimate += timeDelta;

        // Update size estimate.
        //
        // Sometimes estimates don't make sense. Don't let the method
        // size go negative.
        int sizeDelta = EstimateSize(context);

        if (m_CurrentSizeEstimate + sizeDelta <= 0)
        {
            sizeDelta = 0;
        }

        // Update the code size estimate.
        m_CurrentSizeEstimate += sizeDelta;
    }
}

//------------------------------------------------------------------------
// BudgetCheck: return true if an inline of this size would likely
//     exceed the jit time budget for this method
//
// Arguments:
//     ilSize - size of the method's IL
//
// Return Value:
//     true if the inline would go over budget
//
// Notes:
//     Presumes all IL in the method will be imported.

bool InlineStrategy::BudgetCheck(unsigned ilSize)
{
    const int  timeDelta = EstimateInlineTime(ilSize);
    const bool result    = (timeDelta + m_CurrentTimeEstimate > m_CurrentTimeBudget);

    if (result)
    {
        JITDUMP("\nBudgetCheck: for IL Size %d, timeDelta %d +  currentEstimate %d > currentBudget %d\n", ilSize,
                timeDelta, m_CurrentTimeEstimate, m_CurrentTimeBudget);
    }

    return result;
}

//------------------------------------------------------------------------
// NewRoot: construct an InlineContext for the root method
//
// Return Value:
//    InlineContext for use as the root context
//
// Notes:
//    We leave m_Code as nullptr here (rather than the IL buffer
//    address of the root method) to preserve existing behavior, which
//    is to allow one recursive inline.

InlineContext* InlineStrategy::NewRoot()
{
    InlineContext* rootContext = new (m_Compiler, CMK_Inlining) InlineContext(this);

    rootContext->m_ILSize = m_Compiler->info.compILCodeSize;
    rootContext->m_Code   = m_Compiler->info.compCode;
    rootContext->m_Callee = m_Compiler->info.compMethodHnd;

    // May fail to block recursion for normal methods
    // Might need the actual context handle here
    rootContext->m_RuntimeContext = METHOD_BEING_COMPILED_CONTEXT();

    return rootContext;
}

InlineContext* InlineStrategy::NewContext(InlineContext* parentContext, Statement* stmt, GenTreeCall* call)
{
    InlineContext* context = new (m_Compiler, CMK_Inlining) InlineContext(this);

    context->m_InlineStrategy = this;
    context->m_Parent         = parentContext;
    context->m_Sibling        = parentContext->m_Child;
    parentContext->m_Child    = context;

    // In debug builds we record inline contexts in all produced calls to be
    // able to show all failed inlines in the inline tree, even non-candidates.
    // These should always match the parent context we are seeing here.
    assert(parentContext == call->gtInlineContext);

    if (call->IsInlineCandidate())
    {
        InlineCandidateInfo* info   = call->GetSingleInlineCandidateInfo();
        context->m_Code             = info->methInfo.ILCode;
        context->m_ILSize           = info->methInfo.ILCodeSize;
        context->m_ActualCallOffset = info->ilOffset;
        context->m_RuntimeContext   = info->exactContextHandle;

#ifdef DEBUG
        // All inline candidates should get their own statements that have
        // appropriate debug info (or no debug info).
        InlineContext* diInlineContext = stmt->GetDebugInfo().GetInlineContext();
        assert(diInlineContext == nullptr || diInlineContext == parentContext);
#endif
    }
    else
    {
        // Should only get here in debug builds
        INDEBUG(context->m_ActualCallOffset = call->gtRawILOffset);
    }

    // We currently store both the statement location (used when reporting
    // only-style mappings) and the actual call offset (used when reporting the
    // inline tree for rich debug info).
    // These are not always the same, consider e.g.
    // ldarg.0
    // call <foo>
    // which becomes a single statement where the IL location points to the
    // ldarg instruction.
    context->m_Location = stmt->GetDebugInfo().GetLocation();

    assert(call->gtCallType == CT_USER_FUNC);
    context->m_Callee = call->gtCallMethHnd;

#if defined(DEBUG)
    context->m_Devirtualized = call->IsDevirtualized();
    context->m_Guarded       = call->IsGuarded();
    context->m_Unboxed       = call->IsUnboxed();
    context->m_TreeID        = call->gtTreeID;
#endif

    return context;
}

void InlineContext::SetSucceeded(const InlineInfo* info)
{
    assert(InlIsValidObservation(info->inlineResult->GetObservation()));
    m_Observation    = info->inlineResult->GetObservation();
    m_ImportedILSize = info->inlineResult->GetImportedILSize();
    m_Success        = true;

#if defined(DEBUG)
    m_Policy           = info->inlineResult->GetPolicy();
    m_CodeSizeEstimate = m_Policy->CodeSizeEstimate();
#endif

    m_Ordinal = m_InlineStrategy->m_InlineCount + 1;

    m_InlineStrategy->NoteOutcome(this);
}

void InlineContext::SetFailed(const InlineResult* result)
{
    assert(InlIsValidObservation(result->GetObservation()));
    m_Observation    = result->GetObservation();
    m_ImportedILSize = result->GetImportedILSize();
    m_Success        = false;

#if defined(DEBUG)
    m_Policy           = result->GetPolicy();
    m_CodeSizeEstimate = m_Policy->CodeSizeEstimate();
#endif

    m_InlineStrategy->NoteOutcome(this);
}

#if defined(DEBUG)

// Static to track emission of the inline data header

bool InlineStrategy::s_HasDumpedDataHeader = false;

// Static to track emission of the xml data header
// and lock to prevent interleaved file writes

bool          InlineStrategy::s_HasDumpedXmlHeader = false;
CritSecObject InlineStrategy::s_XmlWriterLock;

//------------------------------------------------------------------------
// GetRandom: setup or access random state
//
// Arguments:
//   seed -- seed value to use if not doing random inlines
//
// Return Value:
//    New or pre-existing random state.
//
// Notes:
//    Random state is kept per jit compilation request. Seed is partially
//    specified externally (via stress or policy setting) and partially
//    specified internally via method hash.

CLRRandom* InlineStrategy::GetRandom(int optionalSeed)
{
    if (m_Random == nullptr)
    {
        int externalSeed = optionalSeed;

#ifdef DEBUG

        if (m_Compiler->compRandomInlineStress())
        {
            externalSeed = getJitStressLevel();
            // We can set DOTNET_JitStressModeNames without setting DOTNET_JitStress,
            // but we need external seed to be non-zero.
            if (externalSeed == 0)
            {
                externalSeed = 2;
            }
        }

#endif // DEBUG

        int randomPolicyFlag = JitConfig.JitInlinePolicyRandom();
        if (randomPolicyFlag != 0)
        {
            externalSeed = randomPolicyFlag;
        }

        int internalSeed = m_Compiler->info.compMethodHash();

        assert(externalSeed != 0);
        assert(internalSeed != 0);

        int seed = externalSeed ^ internalSeed;

        JITDUMP("\n*** Using random seed ext(%u) ^ int(%u) = %u\n", externalSeed, internalSeed, seed);

        m_Random = new (m_Compiler, CMK_Inlining) CLRRandom();
        m_Random->Init(seed);
    }

    return m_Random;
}

#endif // defined(DEBUG)

//------------------------------------------------------------------------
// IsInliningDisabled: allow strategy to disable inlining in the method being jitted
//
// Notes:
//    Only will return true in debug or special release builds.
//    Expects JitNoInlineRange to be set to the hashes of methods
//    where inlining is disabled.

bool InlineStrategy::IsInliningDisabled()
{

#if defined(DEBUG)

    static ConfigMethodRange range;
    const char*              noInlineRange = JitConfig.JitNoInlineRange();

    if (noInlineRange == nullptr)
    {
        return false;
    }

    // If we have a config string we have at least one entry.  Count
    // number of spaces in our config string to see if there are
    // more. Number of ranges we need is 2x that value.
    unsigned entryCount = 1;
    for (const char* p = noInlineRange; *p != 0; p++)
    {
        if (*p == ' ')
        {
            entryCount++;
        }
    }

    range.EnsureInit(noInlineRange, 2 * entryCount);
    assert(!range.Error());

    return range.Contains(m_Compiler->info.compMethodHash());

#else

    return false;

#endif // defined(DEBUG)
}
