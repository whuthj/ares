#pragma once

namespace aspect
{
    template<typename T> using identity_t = T;

    template<typename Func, typename... Args>
    struct AspectUtils
    {
        AspectUtils(const AspectUtils&) = delete;
        AspectUtils& operator = (const AspectUtils&) = delete;

        AspectUtils(const Func& f) : m_func(f) {}

        template<typename Head>
        auto Invoke(Args&&... args, Head&& aspect)->decltype(m_func(std::forward<Args>(args)...))
        {
            aspect.Before(std::forward<Args>(args)...);
            auto ret = m_func(std::forward<Args>(args)...);
            aspect.After(std::forward<Args>(args)..., ret);
            return ret;
        }

        template<typename Head, typename... Tail>
        auto Invoke(Args&&... args, Head&& aspect, Tail&&... tail)->decltype(m_func(std::forward<Args>(args)...))
        {
            aspect.Before(std::forward<Args>(args)...);
            auto ret = Invoke(std::forward<Args>(args)..., std::forward<Tail>(tail)...);
            aspect.After(std::forward<Args>(args)..., ret);
            return ret;
        }

        template<typename Head>
        void VoidInvoke(Args&&... args, Head&& aspect)
        {
            aspect.Before(std::forward<Args>(args)...);
            m_func(std::forward<Args>(args)...);
            aspect.After(std::forward<Args>(args)...);
        }

        template<typename Head, typename... Tail>
        void VoidInvoke(Args&&... args, Head&& aspect, Tail&&... tail)
        {
            aspect.Before(std::forward<Args>(args)...);
            VoidInvoke(std::forward<Args>(args)..., std::forward<Tail>(tail)...);
            aspect.After(std::forward<Args>(args)...);
        }

    private:
        Func m_func;
    };

    template<typename... AP, typename... Args, typename Func>
    auto Invoke(Func&& f, Args&&... args)->decltype(f(std::forward<Args>(args)...))
    {
        AspectUtils<Func, Args...> asp(std::forward<Func>(f));
        return asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
    }

    template<typename... AP, typename... Args, typename Func>
    void VoidInvoke(Func&& f, Args&&... args)
    {
        AspectUtils<Func, Args...> asp(std::forward<Func>(f));
        asp.VoidInvoke(std::forward<Args>(args)..., identity_t<AP>()...);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //成员函数切面

    template<typename T, typename Func, typename... Args>
    struct MemberAspectUtils
    {
        MemberAspectUtils(const MemberAspectUtils&) = delete;
        MemberAspectUtils& operator = (const MemberAspectUtils&) = delete;
        MemberAspectUtils(T* pObj, Func pFunc) : m_pObj(pObj), m_pFunc(pFunc) {}

        template<typename Head>
        auto Invoke(Args&&... args, Head&& aspect)->decltype((m_pObj->*m_pFunc)(std::forward<Args>(args)...))
        {
            aspect.Before(std::forward<Args>(args)...);
            auto ret = (m_pObj->*m_pFunc)(std::forward<Args>(args)...);
            aspect.After(std::forward<Args>(args)..., ret);
            return ret;
        }

        template<typename Head, typename... Tail>
        auto Invoke(Args&&... args, Head&& aspect, Tail&&... tail)->decltype((m_pObj->*m_pFunc)(std::forward<Args>(args)...))
        {
            aspect.Before(std::forward<Args>(args)...);
            auto ret = Invoke(std::forward<Args>(args)..., std::forward<Tail>(tail)...);
            aspect.After(std::forward<Args>(args)..., ret);
            return ret;
        }

        template<typename Head>
        void VoidInvoke(Args&&... args, Head&& aspect)
        {
            aspect.Before(std::forward<Args>(args)...);
            (m_pObj->*m_pFunc)(std::forward<Args>(args)...);
            aspect.After(std::forward<Args>(args)...);
        }

        template<typename Head, typename... Tail>
        void VoidInvoke(Args&&... args, Head&& aspect, Tail&&... tail)
        {
            aspect.Before(std::forward<Args>(args)...);
            VoidInvoke(std::forward<Args>(args)..., std::forward<Tail>(tail)...);
            aspect.After(std::forward<Args>(args)...);
        }

    private:
        Func m_pFunc;
        T* m_pObj;
    };

    template<typename... AP, typename... Args, typename T, typename Func>
    auto MemberInvoke(T* pObj, Func pFunc, Args&&... args)->decltype((pObj->*pFunc)(std::forward<Args>(args)...))
    {
        MemberAspectUtils<T, Func, Args...> asp(pObj, pFunc);
        return asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
    }

    template<typename... AP, typename... Args, typename T, typename Func>
    void MemberVoidInvoke(T* pObj, Func pFunc, Args&&... args)
    {
        MemberAspectUtils<T, Func, Args...> asp(pObj, pFunc);
        asp.VoidInvoke(std::forward<Args>(args)..., identity_t<AP>()...);
    }
}