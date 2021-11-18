/*
 * devcall.h - device callback delegate package
 *
 *  Created on: Oct 20, 2021
 *      Author: Tim Stark
 */

#pragma once

typedef DeviceDelegate<int ()> readLineDelegate;
typedef DeviceDelegate<void (int)> writeLineDelegate;

namespace aspace {
    template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<readLineDelegate, std::remove_reference_t<T> > > >
		{ using type = readLineDelegate; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
    template <typename T> struct delegate_rw_type<T, std::void_t<device_class_rw_t<writeLineDelegate, std::remove_reference_t<T> > > >
		{ using type = writeLineDelegate; using device_class = device_class_rw_t<type, std::remove_reference_t<T> >; };
}

class DeviceCallbackBase
{
public:
    DeviceCallbackBase(Device &owner) : owner(owner) {}
    virtual ~DeviceCallbackBase() = default;

    template <typename T, typename U, typename Enable = void> struct intermediate;
    template <typename T, typename U> struct intermediate<T, U, std::enable_if_t<sizeof(T) >= sizeof(U)>> { using type = T; };
    template <typename T, typename U> struct intermediate<T, U, std::enable_if_t<sizeof(T) < sizeof(U)>> { using type = U; };
    template <typename T, typename U> using intermediate_t = typename intermediate<T, U>::type;
    template <typename T, typename U> using mask_t = std::make_unsigned_t<intermediate_t<T, U>>;

    template <typename Input, typename Result, typename Func, typename Enable = void>
        struct is_transform_form1 : public std::false_type { };
    template <typename Input, typename Result, typename Func, typename Enable = void>
        struct is_transform_form2 : public std::false_type { };
    template <typename Input, typename Result, typename Func, typename Enable = void>
        struct is_transform_form3 : public std::false_type { };
      
    template <typename Input, typename Result, typename Func>
        struct is_transform_form1<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t &, Input, std::make_unsigned_t<Input> &>, Result>::value> > : public std::true_type { };
    template <typename Input, typename Result, typename Func>
        struct is_transform_form2<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, offs_t &, Input>, Result>::value> > : public std::true_type { };
    template <typename Input, typename Result, typename Func>
        struct is_transform_form3<Input, Result, Func, std::enable_if_t<std::is_convertible<std::invoke_result_t<Func, Input>, Result>::value> > : public std::true_type { };

    template <typename Input, typename Result, typename Func>
        struct is_transform : public std::bool_constant<
            is_transform_form1<Input, Result, Func>::value ||
            is_transform_form2<Input, Result, Func>::value ||
            is_transform_form3<Input, Result, Func>::value> { };

    template <typename Input, typename Result, typename Func, typename Enable = void>
        struct transform_result;
    template <typename Input, typename Result, typename Func>
        struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form1<Input, Result, Func>::value> >
            { using type = std::invoke_result_t<Func, offs_t &, Input, std::make_unsigned_t<Input> &>; };
    template <typename Input, typename Result, typename Func>
        struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form2<Input, Result, Func>::value> >
            { using type = std::invoke_result_t<Func, offs_t &, Input>; };
    template <typename Input, typename Result, typename Func>
        struct transform_result<Input, Result, Func, std::enable_if_t<is_transform_form3<Input, Result, Func>::value> >
            { using type = std::invoke_result_t<Func, Input>; };
    template <typename Input, typename Result, typename Func>
        using transform_result_t = typename transform_result<Input, Result, Func>::type;

    template <typename T> using delegate_type_t = aspace::delegate_rw_t<T>;
    template <typename T> using delegate_device_class_t = aspace::delegate_rw_device_class_t<T>;

    inline Device &getOwner() { return owner; }

    template <typename T, typename Impl>
    class TransformBase
    {
    public:
        Impl &setXOR(std::make_unsigned_t<T> val)  { xorValue ^= val; return static_cast<Impl &>(*this); }
        Impl &setMask(std::make_unsigned_t<T> val)  { maskValue = flagMask ? val : (maskValue & val); flagMask = false; return static_cast<Impl &>(*this); }
        Impl &setInvert(std::make_unsigned_t<T> val) { return setXOR(~std::make_unsigned_t<T>(0)); }

        constexpr std::make_unsigned_t<T> getMask() const { return maskValue; }
        constexpr std::make_unsigned_t<T> getXOR() const  { return xorValue & maskValue; }

    protected:
        TransformBase(std::make_unsigned_t<T> mask) : maskValue(mask) {}

    private:
        std::make_unsigned_t<T> maskValue;
        std::make_unsigned_t<T> xorValue = std::make_unsigned_t<T>(0);
        bool flagMask = true;
    };

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
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form1<Result, Func>::value>>
        { using type = std::invoke_result_t<Func, offs_t, std::make_unsigned<Result>>; };
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form2<Result, Func>::value>>
        { using type = std::invoke_result_t<Func, offs_t>; };
    template <typename Result, typename Func> struct read_result<Result, Func, std::enable_if_t<is_read_form3<Result, Func>::value>>
        { using type = std::invoke_result_t<Func>; };
    template <typename Result, typename Func> using read_result_t = typename read_result<Result, Func>::type;

    template <typename Result, typename T> static std::enable_if_t<is_read_form1<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
        invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
        {
            return std::make_unsigned_t<read_result_t<Result, T>>(cb(offset, memMask));
        }

    template <typename Result, typename T> static std::enable_if_t<is_read_form2<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
        invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
        {
            return std::make_unsigned_t<read_result_t<Result, T>>(cb(offset));
        }

    template <typename Result, typename T> static std::enable_if_t<is_read_form3<Result, T>::value, mask_t<read_result_t<Result, T>, Result>>
        invokeRead(T const &cb, offs_t offset, std::make_unsigned_t<Result> memMask)
        {
            return std::make_unsigned_t<read_result_t<Result, T>>(cb());
        }

    template <typename T, typename Dummy = void> struct delegate_traits;

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

    class Creator
    {
    public:
        using ptr = std::unique_ptr<Creator>;

        virtual ~Creator() = default;

        virtual func_t create() = 0;

        std::make_unsigned_t<Result> getMask() const { return mask; }

    protected:
        Creator(std::make_unsigned_t<Result> mssk) : mask(mask) {}

        std::make_unsigned_t<Result> mask;
    };

    template <typename T>
    class CreatorBase : public Creator
    {
    public:
        CreatorBase(T &&builder) : Creator(builder.getMask()), builder(std::move(builder)) {}

        virtual func_t create() override
        {
            func_t newFunc = nullptr;
            builder.build(
                [&newFunc](auto &&f)
                {
                    newFunc = [cb = std::move(f)](offs_t offset, typename T::input_mask_t memMask)
                    {
                        return cb(offset, memMask);
                    };
                }
            );
            return newFunc;
        }

    private:
        T builder;
    };

    // template <typename Source, typename Func>
    // class TransformBuilder;

    class BuilderBase
    {
    protected:
        BuilderBase(cbRead_t &cb, bool append)
        : target(cb), append(append)
        { }
        BuilderBase(BuilderBase &&) = default;
        BuilderBase(BuilderBase const &) = delete;
        ~BuilderBase() { assert(consumed); }

        void consume() { consumed = true; }

        template <typename T>
        void registerCreator()
        {
            if (consumed == false)
            {
                if (append == false)
                    target.creators.clear();
                consumed = true;
                target.creators.emplace_back(std::make_unique<CreatorBase<T>>(std::move(static_cast<T &>(*this))));
            }
        }

        cbRead_t &target;
        bool append;
        bool consumed = false;
    };

    // template <typename Source, typename Func>
    // class TransformBuilder :
    //     public BuilderBase,
    //     public TransformBase<mask_t<transform_result_t<typename Source::output_t, Result, Func>, Result>,
    //         TransformBuilder<Source, Func> >
    // {
    // public:

    //     using input_t = typename Source::output_t;
    //     using output_t = mask_t<transform_result_t<input_t, Result, Func>, Result>;
    //     using input_mask_t = mask_t<input_t, typename Source::input_mask_t>;

    //     // template <typename T>
    //     // TransformBuilder(cdRead_t &target, bool append), Source &&src, T &&cb, output_t mask)
    //     // : BuilderBase(target, append), TransformBase<output_t, TransformBuilder>(mask),
    //     //   source(std::move(src)), cb(std::forward<T>(cb))
    //     // {
    //     //     src.consume();
    //     // }

    //     // ~TransformBuilder() { this->template registerCreator<TransformBuilder>(); }
    
    // private:
    //     Source source;
    //     Func cb;
    // };

    template <typename Delegate>
    class DelegateBuilder
    : public BuilderBase,
      public TransformBase<mask_t<read_result_t<Result, Delegate>, Result>, DelegateBuilder<Delegate>>
    {
    public:
        using output_t = mask_t<read_result_t<Result, Delegate>, Result>;
        using input_mask_t = std::make_unsigned_t<Result>;

        template <typename T>
        DelegateBuilder(cbRead_t &cb, bool append, Device &device, ctag_t *devName, T &&func, ctag_t *funcName)
        : BuilderBase(cb, append), 
          TransformBase<output_t, DelegateBuilder<Delegate>>(DefaultMask),
          delegate(device, devName, std::forward<T>(func), funcName)
        {
        }

        DelegateBuilder(DelegateBuilder &&that)
        : BuilderBase(std::move(that)),
          TransformBase<output_t, DelegateBuilder>(std::move(that)),
          delegate(std::move(that.delegate))
        {
            that.consume();
        }

        ~DelegateBuilder()
        {
            this->template registerCreator<DelegateBuilder>();
        }

        template <typename T>
        void build(T &&chain)
        {
            assert(this->consumed);
            delegate.resolve();
            chain(
                [cb = std::move(this->delegate), exor = this->getXOR(), mask = this->getMask()](offs_t offset, input_mask_t memMask)
                {
                    return (cbRead_t::invokeRead<Result>(cb, offset, memMask & mask) ^ exor) & mask;
                }
            );
        }

    private:
        Delegate delegate;
    };

    class Binder
    {
    public:
        Binder(cbRead_t &cb) : target(cb) {}
        
        template <typename T> void set(T &&func, ctag_t *name)
        {
            setUsed();

            Device &dev  = target.getOwner();
            Device &cdev = *dev.getSystemConfig().getConfigDevice();
            fmt::printf("%s(%llx): Setting %s on device %s\n", cdev.getDeviceName(), offs_t(&cdev), name, dev.getDeviceName());
            DelegateBuilder<delegate_type_t<T>>(target, append, cdev, "", std::forward<T>(func), name);
        }

    private:
        void setUsed() { assert(!flagUsed); flagUsed = true; }

    private:
        cbRead_t &target;

        bool append = false;
        bool flagUsed = false;
    };

public:
    cbRead_t(Device &owner) : DeviceCallbackReadBase(owner) {}

    inline Binder bind() { return Binder(*this); }
    inline bool isNull() { return functions.empty(); }

    void resolveSafe(Result defaultValue)
    {
        resolve();
        if (!functions.empty())
            return;

        functions.emplace_back(
            [defaultValue](offs_t offset, std::make_unsigned_t<Result> memMask)
            {
                return defaultValue;
            }
        );
    }

    void resolve()
    {
        assert(functions.empty());
        functions.reserve(creators.size());
        for (typename Creator::ptr const &creator : creators)
            functions.emplace_back(creator->create());
        creators.clear();
    }

    Result operator () (offs_t offset, std::make_unsigned_t<Result> memMask = DefaultMask)
    {
        assert(creators.empty() && !functions.empty());

        typename std::vector<func_t>::const_iterator it(functions.begin());
        std::make_unsigned_t<Result> data((*it)(offset, memMask));
        while (functions.end() != ++it)
            data |= (*it)(offset, memMask);
        return data;
    }

    Result operator () ()
    {
        return this->operator()(0u, DefaultMask);
    }

private:
    vector<typename Creator::ptr> creators;
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

    class Creator
    {
    public:
        virtual ~Creator() = default;

        virtual func_t create() = 0;
    };

    template <typename T>
    class CreatorBase : public Creator
    {
    public:
        CreatorBase(T &&builder) : builder(builder) {}

        virtual func_t create() override
        {
            return nullptr;
        }

    private:
        T &builder;
    };

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

    void resolve() {}

private:
    vector<Creator> creators;
    vector<func_t> functions;
};

// template <typename Input, std::make_unsigned_t<Input> DefaultMask>
// cbWrite_t<Input, DefaultMask>::cbWrite_t(Device &owner)
// : DeviceCallbackWriteBase(owner)
// { }

using read8cb_t = cbRead_t<uint8_t>;
using read16cb_t = cbRead_t<uint16_t>;
using read32cb_t = cbRead_t<uint32_t>;
using read64cb_t = cbRead_t<uint64_t>;
using readlcb_t = cbRead_t<uint8_t, 1u>;

using write8cb_t = cbWrite_t<uint8_t>;
using write16cb_t = cbWrite_t<uint16_t>;
using write32cb_t = cbWrite_t<uint32_t>;
using write64cb_t = cbWrite_t<uint64_t>;
using writelcb_t = cbWrite_t<uint8_t, 1u>;
