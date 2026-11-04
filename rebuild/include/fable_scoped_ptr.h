#pragma once

// The subset of boost::scoped_ptr (boost 1.3x) that retail inlines.
namespace boost
{
    template<class T> inline void checked_delete(T* x)
    {
        typedef char type_must_be_complete[sizeof(T)];
        delete x;
    }

    template<class T> class scoped_ptr
    {
    public:
        explicit scoped_ptr(T* p = 0)
            : ptr(p)
        {
        }

        ~scoped_ptr()
        {
            checked_delete(ptr);
        }

        void reset(T* p = 0)
        {
            scoped_ptr<T>(p).swap(*this);
        }

        T& operator*() const
        {
            return *ptr;
        }

        T* operator->() const
        {
            return ptr;
        }

        T* get() const
        {
            return ptr;
        }

        // boost 1.3x's safe-bool conversion. Retail tests `if (PWorld)` as
        // `neg; sbb; test reg, &scoped_ptr<CWorld>::get` (CMainGameComponent::
        // Shutdown 0x004175E5 against 0x00418F47).
        typedef T* (scoped_ptr::*unspecified_bool_type)() const;

        operator unspecified_bool_type() const
        {
            return ptr == 0 ? 0 : &scoped_ptr<T>::get;
        }

        void swap(scoped_ptr& b)
        {
            T* tmp = b.ptr;
            b.ptr = ptr;
            ptr = tmp;
        }

    private:
        scoped_ptr(const scoped_ptr&);
        scoped_ptr& operator=(const scoped_ptr&);

        T* ptr;
    };
}
