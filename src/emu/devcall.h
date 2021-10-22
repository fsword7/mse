/*
 * devcall.h - device callback delegate package
 *
 *  Created on: Oct 20, 2021
 *      Author: Tim Stark
 */

#pragma once

class DeviceCallbackBase
{
public:
    DeviceCallbackBase(Device &owner) : owner(owner) {}
    virtual ~DeviceCallbackBase() = default;

private:
    Device &owner;
};

// **********************************************************************

class DeviceCallbackReadBase : public DeviceCallbackBase
{
protected:
    using DeviceCallbackBase::DeviceCallbackBase;
    ~DeviceCallbackReadBase() = default; 

    template <typename Result, typename Func, typename Enable = void> struct is_read_form1 : public std::false_type {};
    template <typename Result, typename Func, typename Enable = void> struct is_read_form2 : public std::false_type {};
    template <typename Result, typename Func, typename Enable = void> struct is_read_form3 : public std::false_type {};
    template <typename Result, typename Func> struct is_read_form1<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t, Result>, Result>::value>> : public std::true_type { };
    template <typename Result, typename Func> struct is_read_form2<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t>, Result>::value>> : public std::true_type { };
    template <typename Result, typename Func> struct is_read_form3<Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func>, Result>::value>> : public std::true_type { };
    template <typename Result, typename Func> struct is_read : public std::bool_constant<is_read_form1<Result, Func>::value || is_read_form2<Result, Func>::value || is_read_form3<Result, Func>::value> {};

    template <typename Result, typename Func, typename Enable = void> struct read_result;
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form1<Result, Func>::value>> { using type = std::invoke_result_t<Func, offs_t, std::make_unsigned<Result>>; };
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form2<Result, Func>::value>> { using type = std::invoke_result_t<Func, offs_t>; };
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form3<Result, Func>::value>> { using type = std::invoke_result_t<Func>; };
    template <typename Result, typename Func> using read_result_t = typename read_result<Result, Func>::type;

};

class DeviceCallbackWriteBase : public DeviceCallbackBase
{
protected:
    using DeviceCallbackBase::DeviceCallbackBase;
    ~DeviceCallbackWriteBase() = default;

    template <typename Input, typename Func, typename Enable = void> struct is_write_form1 : public std::false_type {};
    template <typename Input, typename Func, typename Enable = void> struct is_write_form2 : public std::false_type {};
    template <typename Input, typename Func, typename Enable = void> struct is_write_form3 : public std::false_type {};
    template <typename Input, typename Func> struct is_write_form1<Input, Func, void_t<std::invoke_result_t<Func, offs_t, Input, std::make_unsigned_t<Input>> > > : public std::true_type { };
    template <typename Input, typename Func> struct is_write_form2<Input, Func, void_t<std::invoke_result_t<Func, offs_t, Input> > > : public std::true_type { };
    template <typename Input, typename Func> struct is_write_form3<Input, Func, void_t<std::invoke_result_t<Func> > > : public std::true_type { };
    template <typename Input, typename Func> struct is_write : public std::bool_constant<is_write_form1<Input, Func>::value || is_write_form2<Input, Func>::value || is_write_form3<Input, Func>::value> {};

};

// **********************************************************************

template <typename Result, std::make_unsigned_t<Result> DefaultMask
    = std::make_unsigned_t<Result>(~std::make_unsigned_t<Result>(0))>
class cbRead_t : public DeviceCallbackReadBase
{
private:
    using func_t = std::function<Result (offs_t, std::make_unsigned_t<Result>)>;

    class BuilderBase
    {
        
    };

    class DelegateBuilder
    {

    };

    class Binder
    {
    public:
        Binder(cbRead_t &cb) : target(cb) {}
        
        template <typename F> void set(F &&callback, ctag_t *name)
        {

        }

    private:
        void setUsed() { assert(!flagUsed); flagUsed = true; }

    private:
        cbRead_t &target;

        bool flagUsed = false;
    };

public:
    cbRead_t(Device &owner) : DeviceCallbackReadBase(owner) {}

    inline Binder bind() { return Binder(*this); }
    inline bool isNull() { return functions.empty(); }

private:
    vector<func_t> functions;
};

// template <typename Result, std::make_unsigned_t<Result> DefaultMask>
// cbRead_t<Result, DefaultMask>::cbRead_t(Device &owner)
// : DeviceCallbackReadBase(owner)
// { }


// **********************************************************************

template <typename Input, std::make_unsigned_t<Input> DefaultMask
    = std::make_unsigned_t<Input>(~std::make_unsigned_t<Input>(0))>
class cbWrite_t : public DeviceCallbackWriteBase
{
private:
    using func_t = std::function<void (offs_t, Input, std::make_unsigned_t<Input>)>;

    class Binder
    {
    public:
        Binder(cbWrite_t &cb) : target(cb) {}
        
    private:
        void setUsed() { assert(!flagUsed); flagUsed = true; }

    private:
        cbWrite_t &target;

        bool flagUsed = false;
    };

public:
    cbWrite_t(Device &owner) : DeviceCallbackWriteBase(owner) {}

    inline Binder bind() { return Binder(*this); }
    inline bool isNull() { return functions.empty(); }

private:
    vector<func_t> functions;
};

// template <typename Input, std::make_unsigned_t<Input> DefaultMask>
// cbWrite_t<Input, DefaultMask>::cbWrite_t(Device &owner)
// : DeviceCallbackWriteBase(owner)
// { }

using cbRead8_t = cbRead_t<uint8_t>;
using cbRead16_t = cbRead_t<uint16_t>;
using cbRead32_t = cbRead_t<uint32_t>;
using cbRead64_t = cbRead_t<uint64_t>;

using cbWrite8_t = cbWrite_t<uint8_t>;
using cbWrite16_t = cbWrite_t<uint16_t>;
using cbWrite32_t = cbWrite_t<uint32_t>;
using cbWrite64_t = cbWrite_t<uint64_t>;
