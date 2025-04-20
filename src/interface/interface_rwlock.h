/*
 * Copyright (c) 2024 kamin.deng
 * Email: kamin.deng@gmail.com
 * Created on 2024/8/23.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef IREAD_WRITE_LOCK_H_
#define IREAD_WRITE_LOCK_H_

namespace osal {

class IRWLock {
public:
    virtual ~IRWLock() = default;

    // 获取读锁
    virtual void readLock() = 0;

    // 尝试获取读锁
    virtual bool tryReadLock() = 0;

    // 带超时的读锁
    virtual bool readLockFor(uint32_t timeout) = 0;

    // 释放读锁
    virtual void readUnlock() = 0;

    // 获取写锁
    virtual void writeLock() = 0;

    // 尝试获取写锁
    virtual bool tryWriteLock() = 0;

    // 带超时的写锁
    virtual bool writeLockFor(uint32_t timeout) = 0;

    // 释放写锁
    virtual void writeUnlock() = 0;

    // 获取当前读锁数量
    [[nodiscard]] virtual size_t getReadLockCount() const = 0;

    // 获取当前写锁状态
    [[nodiscard]] virtual bool isWriteLocked() const = 0;
};

}  // namespace osal
#endif  // IREAD_WRITE_LOCK_H_
