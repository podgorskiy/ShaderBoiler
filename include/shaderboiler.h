#pragma once
#include <sstream>
#include <memory>
#include <set>
#include <list>
#include <vector>

namespace sb
{
	enum Type
	{
		variable,
		in,
		out
	};

	class node;

	typedef std::shared_ptr<node>nodePtr;

	class node
	{
	public:
		enum DataSize
		{
			size1 = 1,
			size2 = 2,
			size3 = 3,
			size4 = 4,
		};

		enum DataType
		{
			bvec,
			ivec,
			uvec,
			dvec,
			vec,
			mat,
			mat2x,
			mat3x,
			mat4x,
			dmat,
			dmat2x,
			dmat3x,
			dmat4x,
		};

		enum OpType
		{
			input,
			output,
			floatConstant,
			integerConstant,
			unsignedIntegerConstant,
			booleanConstant,
			addition,
			substruction,
			multiplication,
			division,
			lshift,
			rshift,
			or ,
			xor,
			and,
			lor,
			lxor,
			land,
			equal,
			mod,
			neg,
			preinc,
			predec,
			postinc,
			postdec,

			ovec = floatConstant,
			oivec = integerConstant,
			ouvec = unsignedIntegerConstant,
			obvec = booleanConstant
		};
		
		union Data
		{
			float d_vec;
			int d_ivec;
			unsigned int d_uvec;
			bool d_bvec;
		};

		int id;
		DataType datatype;
		DataSize datasize;
		Data x;
		Data y;
		Data z;
		Data w;
		OpType optype;
		std::vector<nodePtr> childs;
	};

	template<node::DataType T, node::DataSize S>
	class basevar
	{
	public:
		basevar(Type t) : type(t)
		{
			src = nodePtr(new node());
			src->datatype = T;
			src->datasize = S;
			if (t == in)
			{
				src->optype = node::input;
			}
		}
		basevar operator =(basevar& other)
		{
			src = other.src;
			return *this;
		}

		nodePtr src;
		Type type;
	private:
	};

	namespace plane_types
	{
		typedef float vec;
		typedef int ivec;
		typedef unsigned int uvec;
		typedef bool bvec;
	}

// All this could be templates, not macroses. But sadly, class template arguments deduction was introduced only in C++17. To have implicit type conversion, argument deduction is needed.
// Once C++17 is supported by all major compilers, this should be rewritten
#define binop(T1, T2, T3, X, E)\
	T3 operator X (const T1& a, const T2& b) { \
		T3 result(Type::variable); \
		result.src->optype = node::##E; \
		result.src->childs.push_back(a.src); \
		result.src->childs.push_back(b.src); \
		return result; \
	}

#define op_ariphm(T1, T2, T3)\
	binop(T1, T2, T3, +, addition); \
	binop(T1, T2, T3, -, substruction); \
	binop(T1, T2, T3, *, multiplication); \
	binop(T1, T2, T3, /, division);
	
#define op_shifts(T1, T2)\
	binop(T1, T2, T3, <<, lshift); \
	binop(T1, T2, T3, >>, rshift);

#define op_bitop(T1, T2, T3)\
	binop(T1, T2, T3, &, and); \
	binop(T1, T2, T3, |, or); \
	binop(T1, T2, T3, ^, xor);
	
#define class_def_size1(T) \
	class T##1: public basevar<node::DataType::##T, node::DataSize(1)>{ \
	public: \
		T##1(Type t) : basevar(t) {}; \
		T##1(plane_types::##T f) : basevar(Type::variable) { src->x.d_##T = f; src->optype = node::OpType::o##T; }; \
	};

#define class_def_size2(T) \
	class T##2: public basevar<node::DataType::##T, node::DataSize(2)>{ \
	public: \
		T##2(Type t) : basevar(t) {}; \
		T##2(plane_types::##T f1, plane_types::##T f2) : basevar(Type::variable) { src->x.d_##T = f1; src->optype = node::OpType::o##T; }; \
	};

#define class_def_size3(T) \
	class T##3: public basevar<node::DataType::##T, node::DataSize(3)>{ \
	public: \
		T##3(Type t) : basevar(t) {}; \
		T##3(plane_types::##T f1, plane_types::##T f2, plane_types::##T f3) : basevar(Type::variable) { src->x.d_##T = f1; src->optype = node::OpType::o##T; }; \
	}; 

#define class_def_size4(T) \
	class T##4: public basevar<node::DataType::##T, node::DataSize(4)>{ \
	public: \
		T##4(Type t) : basevar(t) {}; \
		T##4(plane_types::##T f1, plane_types::##T f2, plane_types::##T f3, plane_types::##T f4) : basevar(Type::variable) { src->x.d_##T = f1; src->optype = node::OpType::o##T; }; \
	};

#define class_def(T) \
	class_def_size1(T) \
	class_def_size2(T) \
	class_def_size3(T) \
	class_def_size4(T)

	class_def(vec);
	class_def(ivec);
	class_def(uvec);
	class_def(bvec);

#define op_ariphm_allTypes(S1, S2, S3) \
	op_ariphm(vec##S1, vec##S2, vec##S3) \
	op_ariphm(bvec##S1, bvec##S2, bvec##S3) \
	op_ariphm(ivec##S1, ivec##S2, ivec##S3) \
	op_ariphm(uvec##S1, uvec##S2, uvec##S3)

	op_ariphm_allTypes(1, 1, 1);
	op_ariphm_allTypes(2, 2, 2);
	op_ariphm_allTypes(3, 3, 3);
	op_ariphm_allTypes(4, 4, 4);

	op_ariphm_allTypes(1, 2, 2);
	op_ariphm_allTypes(1, 3, 3);
	op_ariphm_allTypes(1, 4, 4);

	op_ariphm_allTypes(2, 1, 2);
	op_ariphm_allTypes(3, 1, 2);
	op_ariphm_allTypes(4, 1, 3);

	binop(bvec1, bvec1, bvec1, &&, land);
	binop(bvec1, bvec1, bvec1, ||, lor);
	//binop(bvec1, bvec1, bvec1, ^^, lxor); // operator not supported by c++

#undef binop
#undef class_def_size1
#undef class_def_size2
#undef class_def_size3
#undef class_def_size4
#undef class_def
#undef op_ariphm
#undef op_ariphm_allTypes
#undef op_bitop
#undef op_shifts

	class context
	{
	public:
		void VisitNode(nodePtr n)
		{
			sortedList.clear();
			visitedNodes.clear();
			VisitNodeInternal(n);
		}

		std::string Emit(nodePtr n)
		{
			n->id = id++;
			std::stringstream ss;
			ss << GetType(n) << " " << GetId(n) << " = ";

			switch (n->optype)
			{
			case node::floatConstant:
			{
				ss << n->x.d_vec;
			}
			break;
			case node::integerConstant:
			{
				ss << n->x.d_ivec;
			}
			break;
			case node::unsignedIntegerConstant:
			{
				ss << n->x.d_uvec << "u";
			}
			break;
			case node::booleanConstant:
			{
				if (n->x.d_bvec)
				{
					ss << "true";
				}
				else
				{
					ss << "false";
				}
			}
			break;
			case node::addition:
			{
				ss << GetId(n->childs[0]) << " + " << GetId(n->childs[1]);
			}
			break;
			case node::substruction:
			{
				ss << GetId(n->childs[0]) << " - " << GetId(n->childs[1]);
			}
			break;
			case node::multiplication:
			{
				ss << GetId(n->childs[0]) << " * " << GetId(n->childs[1]);
			}
			break;
			case node::division:
			{
				ss << GetId(n->childs[0]) << " / " << GetId(n->childs[1]);
			}
			break;
			case node::lshift:
			{
				ss << GetId(n->childs[0]) << " << " << GetId(n->childs[1]);
			}
			case node::rshift:
			{
				ss << GetId(n->childs[0]) << " >> " << GetId(n->childs[1]);
			}
			case node:: or :
			{
				ss << GetId(n->childs[0]) << " | " << GetId(n->childs[1]);
			}
			case node::xor:
			{
				ss << GetId(n->childs[0]) << " ^ " << GetId(n->childs[1]);
			}
			case node::and:
			{
				ss << GetId(n->childs[0]) << " & " << GetId(n->childs[1]);
			}
			case node::lor:
			{
				ss << GetId(n->childs[0]) << " || " << GetId(n->childs[1]);
			}
			case node::lxor:
			{
				ss << GetId(n->childs[0]) << " ^^ " << GetId(n->childs[1]);
			}
			case node::land:
			{
				ss << GetId(n->childs[0]) << " && " << GetId(n->childs[1]);
			}
			case node::equal:
			{
				ss << GetId(n->childs[0]) << " == " << GetId(n->childs[1]);
			}
			case node::mod:
			{
				ss << GetId(n->childs[0]) << " % " << GetId(n->childs[1]);
			}
			case node::neg:
			{
				ss << "-" << GetId(n->childs[0]);
			}
			case node::preinc:
			{
				ss << "++" << GetId(n->childs[0]);
			}
			case node::predec:
			{
				ss << "--" << GetId(n->childs[0]);
			}
			case node::postinc:
			{
				ss << GetId(n->childs[0]) << "++";
			}
			case node::postdec:
			{
				ss << GetId(n->childs[0]) << "--";
			}
			}

			ss << ";\n";

			return ss.str();
		}

		std::string GenerateCode()
		{
			std::stringstream ss;

			for (nodePtr n : sortedList)
			{
				ss << Emit(n);
			}

			return ss.str();
		}

	private:
		void VisitNodeInternal(nodePtr n)
		{
			if (visitedNodes.find(n) == visitedNodes.end())
			{
				for (auto& child : n->childs)
				{
					VisitNodeInternal(child);
				}
				sortedList.push_back(n);
				visitedNodes.insert(n);
			}
		}

		std::string GetType(nodePtr n)
		{
			std::string size;
			switch (n->datasize)
			{
			case node::DataSize::size1: size = "1"; break;
			case node::DataSize::size2: size = "2"; break;
			case node::DataSize::size3: size = "3"; break;
			case node::DataSize::size4: size = "4"; break;
			}

			if (n->datasize == node::DataSize::size1)
			{
				switch (n->datatype)
				{
				case node::DataType::bvec: return "bool";
				case node::DataType::ivec: return "int";
				case node::DataType::uvec: return "uint";
				case node::DataType::dvec: return "double";
				case node::DataType::vec: return "float";
				}
			}

			switch (n->datatype)
			{
			case node::DataType::bvec: return "bvec" + size;
			case node::DataType::ivec: return "ivec" + size;
			case node::DataType::uvec: return "uvec" + size;
			case node::DataType::dvec: return "dvec" + size;
			case node::DataType::vec: return "vec" + size;

			case node::DataType::mat: return "mat" + size;
			case node::DataType::mat2x: return "mat2x" + size;
			case node::DataType::mat3x: return "mat3x" + size;
			case node::DataType::mat4x: return "mat4x" + size;
			case node::DataType::dmat: return "dmat" + size;
			case node::DataType::dmat2x: return "dmat2x" + size;
			case node::DataType::dmat3x: return "dmat3x" + size;
			case node::DataType::dmat4x: return "dmat4x" + size;
			}
			return "void";
		}

		std::string GetId(nodePtr n)
		{
			return GetId(n->id);
		}

		std::string GetId(int n)
		{
			int base = 'z' - 'a' + 1;

			if (n == 0)
			{
				return "a";
			}

			std::string name;
			while (n != 0)
			{
				int a = n % base;
				n /= base;
				name += 'a' + char(a);
			}

			return name;
		}

		std::list<nodePtr> sortedList;
		std::set<nodePtr> visitedNodes;
		int id = 0;
	};

	inline std::string genShader(vec2 v)
	{
		context ctx;
		ctx.VisitNode(v.src);
		return ctx.GenerateCode();
	}
}
