/*
 * delegate.h - callback function package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

using GenericFunc = void(*)();
class GenericClass;

class BindedDelegate
{
public:
	BindedDelegate() = default;
	virtual ~BindedDelegate() = default;
};

template <typename Class, typename Return, typename... Args>
struct DelegateTraits
{
	using static_ptr_func  = Return (*)(Class *, Args...);
	using static_ref_func  = Return (&)(Class *, Args...);
	using member_ptr_func  = Return (Class::*)(Args...);
	using member_ptr_cfunc = Return (Class::*)(Args...) const;
};

class mfpDelegate
{
public:
	mfpDelegate() = default;

	template<typename MemberFunctionType, typename MemberFunctionClass, typename MemberReturnType, typename StaticFuncType>
	mfpDelegate(MemberFunctionType mfp, MemberFunctionClass *, MemberReturnType *, StaticFuncType)
	{
		*reinterpret_cast<MemberFunctionType *>(this) = mfp;
	}

	bool isNull() { return (func == 0 && thisDelta == 0); }

	template <typename FunctionType>
	void update(FunctionType &func, GenericClass *&obj)
	{
		func = reinterpret_cast<FunctionType>(convertGeneric(obj));

	}
private:
	GenericFunc convertGeneric(GenericClass *&obj) const
	{
		obj = reinterpret_cast<GenericClass *>(reinterpret_cast<uint8_t *>(obj) + thisDelta);

		if ((func & 1) == 0)
			return reinterpret_cast<GenericFunc>(func);

		uint8_t *base = *reinterpret_cast<uint8_t **>(obj);

		return *reinterpret_cast<GenericFunc>(base + func - 1);
	}

	uintptr_t func = 0;
	int       thisDelta = 0;
};

template <typename Return, typename... Args>
class DelegateBase
{
public:
	template <typename FunctionCall>
		using traits = DelegateTraits<FunctionCall, Return, Args...>;
	using GenericStaticFunc = typename traits<GenericClass>::static_ptr_func;
	using GenericMemberFunc = GenericStaticFunc;

	using stdFuncType = std::function<Return (Args...)>;

	DelegateBase() = default;
	DelegateBase(stdFuncType func) : function(func) {}

	template <class FunctionClass>
	DelegateBase(typename traits<FunctionClass>::member_ptr_func func, FunctionClass *obj)
	: mfp(func, obj, static_cast<Return *>(nullptr), static_cast<GenericStaticFunc>(nullptr)),
	  binder(&bindHelper<FunctionClass>)
	{
		bind(reinterpret_cast<GenericClass *>(obj));
	}

	template <class FunctionClass>
	DelegateBase(typename traits<FunctionClass>::member_ptr_cfunc func, FunctionClass *obj)
	: mfp(func, obj, static_cast<Return *>(nullptr), static_cast<GenericStaticFunc>(nullptr)),
	  binder(&bindHelper<FunctionClass>)
	{
		bind(reinterpret_cast<GenericClass *>(obj));
	}

	template <class FunctionClass>
	DelegateBase(typename traits<FunctionClass>::static_ref_func func, FunctionClass *obj)
	: function(reinterpret_cast<GenericStaticFunc>(func)),
	  binder(&bindHelper<FunctionClass>)
	{
		bind(reinterpret_cast<GenericClass *>(obj));
	}

	bool isMFP()  { return !mfp.isNull(); }
	bool isNull() { return (function == nullptr) && (stdfunc == nullptr) && mfp.isNull(); }

	void bind(BindedDelegate &obj)
	{
		if (binder != nullptr)
			bind((*binder)(obj));
	}

	Return operator ()(Args... args) const
	{
		if (stdfunc != nullptr)
			return stdfunc(forward<Args>(args)...);
		else
			return (*function)(object, forward<Args>(args)...);
	}

protected:
	using BindFunc = GenericClass*(*)(BindedDelegate &obj);

	template <class FunctionClass>
	static GenericClass *bindHelper(BindedDelegate &obj)
	{
		FunctionClass *result = dynamic_cast<FunctionClass *>(&obj);
//		if (result == nullptr);

		return reinterpret_cast<GenericClass *>(result);
	}

	void bind(GenericClass *obj)
	{
		object = obj;
		if (object != nullptr && !mfp.isNull())
			mfp.update(function, object);
	}

protected:
	GenericClass      *object = nullptr;
	GenericStaticFunc  function = nullptr;
	stdFuncType        stdfunc = nullptr; // std::function
	BindFunc           binder = nullptr;
	mfpDelegate        mfp;
};

template <typename Signature>
class Delegate;

template <typename Return, typename... Args>
class Delegate<Return (Args...)> : public DelegateBase<Return, Args...>
{
private:
	using base = DelegateBase<Return, Args...>;

protected:
	template <class FunctionClass> using traits = typename base::template traits<FunctionClass>;
	template <class FunctionClass> using member_ptr_func = typename traits<FunctionClass>::member_ptr_func;
	template <class FunctionClass> using member_ptr_cfunc = typename traits<FunctionClass>::member_ptr_cfunc;
	template <class FunctionClass> using static_ref_func = typename traits<FunctionClass>::static_ref_func;

public:
	Delegate() : base() {}

	Delegate(const base &src) : base(src) {}
	Delegate(const base &src, BindedDelegate &obj) : base (src, obj) {}

	explicit Delegate(function<Return (Args...)> func) : base(func) {}

	template <class FunctionClass>
	Delegate(member_ptr_func<FunctionClass> func, FunctionClass *obj)
	: base(func, obj) {}

	template <class FunctionClass>
	Delegate(member_ptr_cfunc<FunctionClass> func, FunctionClass *obj)
	: base(func, obj) {}

	template <class FunctionClass>
	Delegate(static_ref_func<FunctionClass> func, FunctionClass *obj)
	: base(func, obj) {}

};

template <typename Signature>
class NamedDelegate : public Delegate<Signature>
{
private:
	using base = Delegate<Signature>;

protected:
	template <class FunctionClass> using member_ptr_func =
			typename base::template member_ptr_func<FunctionClass>;
	template <class FunctionClass> using member_ptr_cfunc =
			typename base::template member_ptr_cfunc<FunctionClass>;
	template <class FunctionClass> using static_ref_func =
			typename base::template static_ref_func<FunctionClass>;

public:
	NamedDelegate() = default;
	NamedDelegate(const NamedDelegate &src) = default;

	template <class FunctionClass>
	NamedDelegate(ctag_t *name, member_ptr_func<FunctionClass> func, FunctionClass *obj)
	: base(func, obj), name(name) {}

	template <class FunctionClass>
	NamedDelegate(ctag_t *name, member_ptr_cfunc<FunctionClass> func, FunctionClass *obj)
	: base(func, obj), name(name) {}

	template <class FunctionClass>
	NamedDelegate(ctag_t *name, static_ref_func<FunctionClass> func, FunctionClass *obj)
	: base(func, obj), name(name) {}

	inline ctag_t *getName() const { return name; }

	NamedDelegate &operator = (const NamedDelegate &src) = default;

private:
//	cstag_t name;
	ctag_t *name = nullptr;
};

template <typename Signature>
class DeviceDelegate : public NamedDelegate<Signature>
{
private:
	using dbase = DeviceDelegate<Signature>;
	using nbase = NamedDelegate<Signature>;

protected:
	template<class FunctionClass> using member_ptr_func =
			typename nbase::template member_ptr_func<FunctionClass>;
	template<class FunctionClass> using member_ptr_cfunc =
			typename nbase::template member_ptr_cfunc<FunctionClass>;
	template<class FunctionClass> using static_ref_func =
			typename nbase::template static_ref_func<FunctionClass>;

public:
	DeviceDelegate() : nbase() {}
	DeviceDelegate(const nbase &src)
	: nbase(src), devName(src.devName) {}

	template <class FunctionClass>
	DeviceDelegate(ctag_t *name, ctag_t *devName, member_ptr_func<FunctionClass> func)
	: nbase(name, func), devName(devName) {}

	template <class FunctionClass>
	DeviceDelegate(ctag_t *name, ctag_t *devName, member_ptr_cfunc<FunctionClass> func)
	: nbase(name, func), devName(devName) {}

	template <class FunctionClass>
	DeviceDelegate(ctag_t *name, ctag_t *devName, static_ref_func<FunctionClass> func)
	: nbase(name, func), devName(devName) {}

	ctag_t *getName() const { return devName; }

private:
	ctag_t *devName = nullptr;
};

typedef DeviceDelegate<int ()> readDelegate;
typedef DeviceDelegate<void (int)> writeDelegate;

template <typename T>
inline readDelegate makeDelegate(T &&func, ctag_t *fncName, ctag_t *devName)
{
	return readDelegate(fncName, devName, func);
}

template <typename T>
inline writeDelegate makeDelegate(T &&func, ctag_t *fncName, ctag_t *devName)
{
	return writeDelegate(fncName, devName, func);
}
