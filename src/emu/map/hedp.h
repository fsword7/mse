/*
 * hedp.h - Handler entry delegate package
 *
 *  Created on: Dec 5, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int dWidth, int aShift, typename Read>
	class HandlerReadDelegate : public HandlerReadAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift>::uintx_t;

		HandlerReadDelegate(AddressSpace *space, uint32_t flags, const Read &delegate)
		: HandlerReadAddress<dWidth, aShift>(space, flags), delegate(delegate)
		{ }

		~HandlerReadDelegate() = default;

		string getName() const { return delegate.getName(); }

		uintx_t read(offs_t offset, cpuDevice *cpu) const override
		{  
            return readi<Read>(offset, 0);
		}

		uintx_t read(offs_t offset, uintx_t mask, cpuDevice *cpu) const override
		{
			return readi<Read>(offset, mask);
		}

    private:

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8d_t>::value ||
            std::is_same<D, read16d_t>::value ||
            std::is_same<D, read32d_t>::value ||
            std::is_same<D, read64d_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8do_t>::value ||
            std::is_same<D, read16do_t>::value ||
            std::is_same<D, read32do_t>::value ||
            std::is_same<D, read64do_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8dmo_t>::value ||
            std::is_same<D, read16dmo_t>::value ||
            std::is_same<D, read32dmo_t>::value ||
            std::is_same<D, read64dmo_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8ds_t>::value ||
            std::is_same<D, read16ds_t>::value ||
            std::is_same<D, read32ds_t>::value ||
            std::is_same<D, read64ds_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8dso_t>::value ||
            std::is_same<D, read16dso_t>::value ||
            std::is_same<D, read32dso_t>::value ||
            std::is_same<D, read64dso_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8dsmo_t>::value ||
            std::is_same<D, read16dsmo_t>::value ||
            std::is_same<D, read32dsmo_t>::value ||
            std::is_same<D, read64dsmo_t>::value,
            uintx_t> readi(offs_t offset, uintx_t mask) const
        {
            return 0;
        }

        Read delegate;
	};

	template <int dWidth, int aShift, typename Write>
	class HandlerWriteDelegate : public HandlerWriteAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift>::uintx_t;

		HandlerWriteDelegate(AddressSpace *space, uint32_t flags, const Write &delegate)
		: HandlerWriteAddress<dWidth, aShift>(space, flags), delegate(delegate)
		{ }

		~HandlerWriteDelegate() = default;

		string getName() const { return delegate.getName(); }

		void write(offs_t offset, uintx_t data, cpuDevice *cpu) const override
		{
            writei<Write>(offset, data, 0);
		}

		void write(offs_t offset, uintx_t data, uintx_t mask, cpuDevice *cpu) const override
		{
            writei<Write>(offset, data, mask);
		}

    private:

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8d_t>::value ||
            std::is_same<D, write16d_t>::value ||
            std::is_same<D, write32d_t>::value ||
            std::is_same<D, write64d_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8do_t>::value ||
            std::is_same<D, write16do_t>::value ||
            std::is_same<D, write32do_t>::value ||
            std::is_same<D, write64do_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8dmo_t>::value ||
            std::is_same<D, write16dmo_t>::value ||
            std::is_same<D, write32dmo_t>::value ||
            std::is_same<D, write64dmo_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8ds_t>::value ||
            std::is_same<D, write16ds_t>::value ||
            std::is_same<D, write32ds_t>::value ||
            std::is_same<D, write64ds_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, write8dso_t>::value ||
            std::is_same<D, write16dso_t>::value ||
            std::is_same<D, write32dso_t>::value ||
            std::is_same<D, write64dso_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }

        template <typename D>
        std::enable_if_t<
            std::is_same<D, read8dsmo_t>::value ||
            std::is_same<D, read16dsmo_t>::value ||
            std::is_same<D, read32dsmo_t>::value ||
            std::is_same<D, read64dsmo_t>::value,
            void> writei(offs_t offset, uintx_t data, uintx_t mask) const
        {
        }
   
        Write delegate;
	};

}
