#include <memory>

namespace bling { namespace core {
	class BlingContext;
	
	class BlingCore
	{
	public:
		BlingCore();
		~BlingCore();
		void initialize();
	private:
		std::unique_ptr<BlingContext> m_context;
	};
}}