//
// Created by kamin.deng on 2024/8/22.
//

#ifndef TEST_FRAMEWORK_CONFIG_H
#define TEST_FRAMEWORK_CONFIG_H

#define TestOSALThreadStartEnabled 1
#define TestOSALThreadStopEnabled 1
#define TestOSALThreadJoinEnabled 1
#define TestOSALThreadDetachEnabled 1
#define TestOSALThreadIsRunningEnabled 1
#define TestOSALThreadSetAndGetPriorityEnabled 1
#define TestOSALThreadSuspendAndResumeEnabled 1

#define TestOSALChronoNowEnabled 1
#define TestOSALChronoElapsedEnabled 1
#define TestOSALChronoToTimeTEnabled 1
#define TestOSALChronoFromTimeTEnabled 1
#define TestOSALChronoToStringEnabled 1

#define TestOSALMutexLockEnabled 1
#define TestOSALMutexUnlockEnabled 1
#define TestOSALMutexTryLockEnabled 1
#define TestOSALMutexTryLockForEnabled 1
#define TestOSALMutexLockAndUnlockEnabled 1

#define TestOSALConditionVariableWaitAndNotifyOneEnabled 1
#define TestOSALConditionVariableWaitForTimeoutEnabled 1
#define TestOSALConditionVariableNotifyAllEnabled 1
#define TestOSALConditionVariableWaitCountEnabled 1

#define TestOSALLockGuardConstructorEnabled 1
#define TestOSALLockGuardDestructorEnabled 1
#define TestOSALLockGuardIsLockedEnabled 1
#define TestOSALLockGuardMultiThreadEnabled 1

#define TestOSALMemoryManagerAllocateEnabled 1
#define TestOSALMemoryManagerDeallocateEnabled 1
#define TestOSALMemoryManagerReallocateEnabled 1
#define TestOSALMemoryManagerAllocateAlignedEnabled 1
#define TestOSALMemoryManagerGetAllocatedSizeEnabled 0

#define TestOSALMessageQueueSendReceiveEnabled 1
#define TestOSALMessageQueueTryReceiveEnabled 1
#define TestOSALMessageQueueReceiveForEnabled 1
#define TestOSALMessageQueueSizeEnabled 1
#define TestOSALMessageQueueClearEnabled 1

#define TestOSALSemaphoreInitEnabled 1
#define TestOSALSemaphoreWaitSignalEnabled 1
#define TestOSALSemaphoreTryWaitEnabled 1
#define TestOSALSemaphoreTryWaitForEnabled 1
#define TestOSALSemaphoreGetValueEnabled 1

#define TestOSALThreadPoolStartStopEnabled 1
#define TestOSALThreadPoolSuspendResumeEnabled 1
#define TestOSALThreadPoolSubmitTaskEnabled 1
#define TestOSALThreadPoolSetPriorityEnabled 1
#define TestOSALThreadPoolGetTaskQueueSizeEnabled 1
#define TestOSALThreadPoolGetActiveThreadCountEnabled 1
#define TestOSALThreadPoolCancelTaskEnabled 1
#define TestOSALThreadPoolSetMaxThreadsEnabled 1
#define TestOSALThreadPoolSetTaskFailureCallbackEnabled 1
#define TestOSALThreadPoolSetMinThreadsEnabled 1

#define TestOSALTimerRepeatEnabled 1
#define TestOSALTimerStartEnabled 1
#define TestOSALTimerStopEnabled 1
#define TestOSALTimerIsRunningEnabled 1
#define TestOSALTimerGetRemainingTimeEnabled 1
#define TestOSALTimerResetEnabled 1

#define TestOSALRWLockReadLockEnabled 1
#define TestOSALRWLockTryReadLockEnabled 1
#define TestOSALRWLockReadLockForEnabled 1
#define TestOSALRWLockWriteLockEnabled 1
#define TestOSALRWLockTryWriteLockEnabled 1
#define TestOSALRWLockWriteLockForEnabled 1
#define TestOSALRWLockGetReadLockCountEnabled 0
#define TestOSALRWLockIsWriteLockedEnabled 0

#define TestOSALSpinLockLockEnabled 1
#define TestOSALSpinLockTryLockEnabled 1
#define TestOSALSpinLockLockForEnabled 1
#define TestOSALSpinLockUnlockEnabled 1
#define TestOSALSpinLockIsLockedEnabled 1

#endif  // TEST_FRAMEWORK_CONFIG_H
