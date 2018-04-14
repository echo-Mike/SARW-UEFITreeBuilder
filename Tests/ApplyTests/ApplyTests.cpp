#include <ClassUtilsLib\mClassUtils.hpp>
#include <utility>
namespace One
{

	struct Type1 {};

	struct Tag0 {};
	struct Tag1 {};

	namespace Helper
	{
		struct AbstractInheritance
		{
			virtual AbstractInheritance* copy() const = 0;

			virtual AbstractInheritance* move() = 0;

			virtual ~AbstractInheritance() {}
		};

		template < typename T >
		struct InheritanceFilter :
			public AbstractInheritance,
			public T
		{};
	}

	typedef Helper::AbstractInheritance* AbstInhPtr_t;

	namespace Two
	{
		template < typename T, typename TagT = void >
		struct Tester :
			public Type1,
			public Helper::InheritanceFilter< TagT >
		{
			typedef Type1 Base;
			typedef Helper::InheritanceFilter< TagT > AbstractBase;

			AbstractBase* copy() const { return new Tester(*this); }

			AbstractBase* move() { return new Tester(std::move(*this)); }
		};

		template < typename T >
		struct Tester< T, void > :
			public Type1
		{};
	}

	namespace Three
	{
		struct Wrapper
		{

			enum { Simple, Extended } headerType;

			Wrapper(Helper::InheritanceFilter< Tag0 >* ptr) : headerType(Simple), m_ptr(ptr) {}

			Wrapper(Helper::InheritanceFilter< Tag1 >* ptr) : headerType(Extended), m_ptr(ptr) {}

			AbstInhPtr_t m_ptr;
		};
	}

	namespace Four
	{
		struct Type0 {};
		struct Type1 {};
		struct Type2 {};
		struct Type3 {};
		struct Type4 {};
		struct Type5 {};
		struct Type6 {};
		struct Type7 {};
		struct Type8 {};
		struct Type9 {};
	}

	namespace Five
	{
		typedef Two::Tester<Four::Type0, Tag0> Group0Type0;
		typedef Two::Tester<Four::Type1, Tag0> Group0Type1;
		typedef Two::Tester<Four::Type2, Tag0> Group0Type2;
		typedef Two::Tester<Four::Type3, Tag0> Group0Type3;
		typedef Two::Tester<Four::Type4, Tag0> Group0Type4;

		typedef Two::Tester<Four::Type5, Tag1> Group1Type0;
		typedef Two::Tester<Four::Type6, Tag1> Group1Type1;
		typedef Two::Tester<Four::Type7, Tag1> Group1Type2;
		typedef Two::Tester<Four::Type8, Tag1> Group1Type3;
		typedef Two::Tester<Four::Type9, Tag1> Group1Type4;
	}
}

int main(void)
{
	using namespace One;

	Five::Group0Type0* instance_0 = new Five::Group0Type0;
	Five::Group1Type0* instance_1 = new Five::Group1Type0;

	Three::Wrapper instance_2 = instance_0;
	Three::Wrapper instance_3 = instance_1;

	delete instance_0;
	delete instance_1;

	return 1;
}