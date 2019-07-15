#pragma once

namespace desktop { namespace core { namespace model {

	template <typename T, typename Parameter>
	class NamedType
	{
	public:
		explicit NamedType(T const& value) : value_(value) {}
		explicit NamedType(T&& value) : value_(std::move(value)) {}
		T& get() { return value_; }
		T const& get() const { return value_; }
	private:
		T value_;
	};
}}}