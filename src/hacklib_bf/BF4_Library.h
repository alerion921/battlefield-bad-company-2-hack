#ifndef BF4_LIBRARY_H
#define BF4_LIBRARY_H

#include <string>


namespace BF4
{
    namespace eastl
    {
        template<typename T>
        class VectorBase
        {
            T *begin, *end, *capacity;
        public:
            size_t size() { return end-begin; }
            size_t max_size() { return capacity-begin; }
            bool empty() { return !begin || end<=begin; }
            T at(size_t index) { return begin[index]; }
            T &operator[] (size_t index) { return begin[index]; }
        };

        template<typename T>
        class vector : public VectorBase<T>
        {
            int *padding;
        };

        class ansi_string : public eastl::VectorBase<char>
        {
        public:
            std::string str()
            {
                std::string ret;

                size_t len = size();
                for (size_t i = 0; i < len; i++)
                    ret += at(i);

                return ret;
            }
        };

        template <class T>
        class WeakPtr
        {
        public:
            bool isNull() const
            {
                return !m_ptr || !*m_ptr;
            }
            T *operator-> ()
            {
                if (isNull())
                    return nullptr;
                return *m_ptr - 1;
            }
            const T *operator-> () const
            {
                if (isNull())
                    return nullptr;
                return *m_ptr - 1;
            }
            T *operator* ()
            {
                if (isNull())
                    return nullptr;
                return *m_ptr - 1;
            }
            const T *operator*() const
            {
                if (isNull())
                    return nullptr;
                return *m_ptr - 1;
            }
        private:
            T **m_ptr;
        };
    }

    namespace fb
    {
        struct Vec2
        {
            float x, y;
        };
        struct Vec3
        {
            float x, y, z, padding;
        };
        struct Vec4
        {
            float x, y, z, w;
        };
        struct LinearTransform
        {
            Vec4 left, up, forward, trans;
        };
    }
}

#endif
