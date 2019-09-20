#pragma once

#include <utility>

#include <nlohmann/detail/conversions/from_json.hpp>
#include <nlohmann/detail/conversions/to_json.hpp>

namespace nlohmann
{

	template<typename, typename>
	struct adl_serializer
	{
		/*!
		@brief convert a JSON value to any value type

		This function is usually called by the `get()` function of the
		@ref basic_json class (either explicit or via conversion operators).

		@param[in] j        JSON value to read from
		@param[in,out] val  value to write to
		*/
		template<typename BasicJsonType, typename ValueType>
		static auto from_json(BasicJsonType&& j, ValueType& val) noexcept(
			noexcept(::nlohmann::from_json(std::forward<BasicJsonType>(j), val)))
		-> decltype(::nlohmann::from_json(std::forward<BasicJsonType>(j), val), void())
		{
			::nlohmann::from_json(std::forward<BasicJsonType>(j), val);
		}

		/*!
		@brief convert any value type to a JSON value

		This function is usually called by the constructors of the @ref basic_json
		class.

		@param[in,out] j  JSON value to write to
		@param[in] val    value to read from
		*/
		template <typename BasicJsonType, typename ValueType>
		static auto to_json(BasicJsonType& j, ValueType&& val) noexcept(
			noexcept(::nlohmann::to_json(j, std::forward<ValueType>(val))))
		-> decltype(::nlohmann::to_json(j, std::forward<ValueType>(val)), void())
		{
			::nlohmann::to_json(j, std::forward<ValueType>(val));
		}

	};

	/* Added support for unique and shared pointers as per https://github.com/nlohmann/json/issues/975 */
	template <typename T>
	struct adl_serializer<std::unique_ptr<T>> {
		static void to_json(json& j, const std::unique_ptr<T>& opt) {
			if (opt.get()) {
				j = *opt;
			}
			else {
				j = nullptr;
			}
		}

		static void from_json(const json& j, std::unique_ptr<T>& opt) {
			opt = std::make_unique(j);
		}
	};

	/* Added support for unique and shared pointers as per https://github.com/nlohmann/json/issues/975 */
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>> {
		static void to_json(json& j, const std::shared_ptr<T>& opt) {
			if (opt.get()) {
				j = *opt;
			}
			else {
				j = nullptr;
			}
		}

		static void from_json(const json& j, std::shared_ptr<T>& opt) {
			opt = std::make_shared<T>(j);
		}
	};

}  // namespace nlohmann
