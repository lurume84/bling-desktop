#include <memory>

namespace blink { namespace core {
	class BlinkContext;
	
	class BlinkCore
	{
	public:
		BlinkCore();
		~BlinkCore();
		void initialize();
	private:
		std::unique_ptr<BlinkContext> m_context;
	};
}}