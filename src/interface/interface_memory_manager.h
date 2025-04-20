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

#ifndef IMEM_POOL_H_
#define IMEM_POOL_H_

namespace osal {

class IMemoryManager {
public:
    virtual ~IMemoryManager() = default;

    virtual bool initialize(size_t block_size, size_t block_count) = 0;

    virtual void *allocate(size_t size) = 0;

    virtual void deallocate(void *ptr) = 0;

    virtual void *reallocate(void *ptr, size_t newSize) = 0;           // 重新分配内存
    virtual void *allocateAligned(size_t size, size_t alignment) = 0;  // 对齐分配内存
    [[nodiscard]] virtual size_t getAllocatedSize() const = 0;              // 获取已分配内存的大小
};

}  // namespace osal

#endif  // IMEM_POOL_H_
