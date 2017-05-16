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
			dmat,
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
			cast,

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
		DataSize datasize_secondary;
		Data data[16];
		OpType optype;
		std::vector<nodePtr> childs;
	};

	template<node::DataType T, node::DataSize S, node::DataSize S2 = node::size1>
	class basevar
	{
	public:
		basevar(Type t) : type(t)
		{
			src = nodePtr(new node());
			src->datatype = T;
			src->datasize = S;
			src->datasize_secondary = S2;
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

#define REPEAT_WITH_ID_AND_COMMA(X, C) REPEAT_IC##C(X)
#define REPEAT_IC1(X) X##0
#define REPEAT_IC2(X) REPEAT_IC1(X), X##1
#define REPEAT_IC3(X) REPEAT_IC2(X), X##2
#define REPEAT_IC4(X) REPEAT_IC3(X), X##3
#define REPEAT_IC5(X) REPEAT_IC4(X), X##4
#define REPEAT_IC6(X) REPEAT_IC5(X), X##5
#define REPEAT_IC7(X) REPEAT_IC6(X), X##6
#define REPEAT_IC8(X) REPEAT_IC7(X), X##7
#define REPEAT_IC9(X) REPEAT_IC8(X), X##8
#define REPEAT_IC10(X) REPEAT_IC9(X), X##9
#define REPEAT_IC11(X) REPEAT_IC10(X), X##10
#define REPEAT_IC12(X) REPEAT_IC11(X), X##11
#define REPEAT_IC13(X) REPEAT_IC12(X), X##12
#define REPEAT_IC14(X) REPEAT_IC13(X), X##13
#define REPEAT_IC15(X) REPEAT_IC14(X), X##14
#define REPEAT_IC16(X) REPEAT_IC15(X), X##15

#define REPEAT_ASSIGNMENT_WITH_ID(X1, X2, X3, C) REPEAT_AWID##C(X1, X2, X3)
#define REPEAT_AWID1(X1, X2, X3) X1##0##X2 = X3##0;
#define REPEAT_AWID2(X1, X2, X3) REPEAT_AWID1(X1, X2, X3)  X1##1##X2 = X3##1;
#define REPEAT_AWID3(X1, X2, X3) REPEAT_AWID2(X1, X2, X3)  X1##2##X2 = X3##2;
#define REPEAT_AWID4(X1, X2, X3) REPEAT_AWID3(X1, X2, X3)  X1##3##X2 = X3##3;
#define REPEAT_AWID5(X1, X2, X3) REPEAT_AWID4(X1, X2, X3)  X1##4##X2 = X3##4;
#define REPEAT_AWID6(X1, X2, X3) REPEAT_AWID5(X1, X2, X3)  X1##5##X2 = X3##5;
#define REPEAT_AWID7(X1, X2, X3) REPEAT_AWID6(X1, X2, X3)  X1##6##X2 = X3##6;
#define REPEAT_AWID8(X1, X2, X3) REPEAT_AWID7(X1, X2, X3)  X1##7##X2 = X3##7;
#define REPEAT_AWID9(X1, X2, X3) REPEAT_AWID8(X1, X2, X3)  X1##8##X2 = X3##8;
#define REPEAT_AWID10(X1, X2, X3) REPEAT_AWID9(X1, X2, X3)  X1##9##X2 = X3##9;
#define REPEAT_AWID11(X1, X2, X3) REPEAT_AWID10(X1, X2, X3)  X1##10##X2 = X3##10;
#define REPEAT_AWID12(X1, X2, X3) REPEAT_AWID11(X1, X2, X3)  X1##11##X2 = X3##11;
#define REPEAT_AWID13(X1, X2, X3) REPEAT_AWID12(X1, X2, X3)  X1##12##X2 = X3##12;
#define REPEAT_AWID14(X1, X2, X3) REPEAT_AWID13(X1, X2, X3)  X1##13##X2 = X3##13;
#define REPEAT_AWID15(X1, X2, X3) REPEAT_AWID14(X1, X2, X3)  X1##14##X2 = X3##14;
#define REPEAT_AWID16(X1, X2, X3) REPEAT_AWID15(X1, X2, X3)  X1##15##X2 = X3##15;

#define REPEAT_ASSIGNMENT(X1, X2, X3, C) REPEAT_A##C(X1, X2, X3)
#define REPEAT_A1(X1, X2, X3) X1##0##X2 = X3;
#define REPEAT_A2(X1, X2, X3) REPEAT_A1(X1, X2, X3)  X1##1##X2 = X3;
#define REPEAT_A3(X1, X2, X3) REPEAT_A2(X1, X2, X3)  X1##2##X2 = X3;
#define REPEAT_A4(X1, X2, X3) REPEAT_A3(X1, X2, X3)  X1##3##X2 = X3;
#define REPEAT_A5(X1, X2, X3) REPEAT_A4(X1, X2, X3)  X1##4##X2 = X3;
#define REPEAT_A6(X1, X2, X3) REPEAT_A5(X1, X2, X3)  X1##5##X2 = X3;
#define REPEAT_A7(X1, X2, X3) REPEAT_A6(X1, X2, X3)  X1##6##X2 = X3;
#define REPEAT_A8(X1, X2, X3) REPEAT_A7(X1, X2, X3)  X1##7##X2 = X3;
#define REPEAT_A9(X1, X2, X3) REPEAT_A8(X1, X2, X3)  X1##8##X2 = X3;
#define REPEAT_A10(X1, X2, X3) REPEAT_A9(X1, X2, X3)  X1##9##X2 = X3;
#define REPEAT_A11(X1, X2, X3) REPEAT_A10(X1, X2, X3)  X1##10##X2 = X3;
#define REPEAT_A12(X1, X2, X3) REPEAT_A11(X1, X2, X3)  X1##11##X2 = X3;
#define REPEAT_A13(X1, X2, X3) REPEAT_A12(X1, X2, X3)  X1##12##X2 = X3;
#define REPEAT_A14(X1, X2, X3) REPEAT_A13(X1, X2, X3)  X1##13##X2 = X3;
#define REPEAT_A15(X1, X2, X3) REPEAT_A14(X1, X2, X3)  X1##14##X2 = X3;
#define REPEAT_A16(X1, X2, X3) REPEAT_A15(X1, X2, X3)  X1##15##X2 = X3;
	
#define MULL(X, Y) MULL##X##Y
#define MULL11 1
#define MULL12 2
#define MULL13 3
#define MULL14 4
#define MULL21 2
#define MULL22 4
#define MULL23 6
#define MULL24 8
#define MULL31 3
#define MULL32 6
#define MULL33 9
#define MULL34 12
#define MULL41 4
#define MULL42 8
#define MULL43 12
#define MULL44 16
	
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

#define cast_from_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of vec1 type */ \
		explicit T##S(T##1 f) : basevar(Type::variable) { \
			src->optype = node::cast; \
			src->childs.push_back(f.src); \
		};

#define cast_from_const_literal_scalar(T, S) \
		/* Initializes each component of the vec<S> with the one argument of POD type*/ \
		explicit T##S(plane_types::##T f) : basevar(Type::variable) {\
			REPEAT_ASSIGNMENT(src->data[, ].d_##T, f, S); \
			src->optype = node::OpType::o##T; \
		};

#define main_constructor(T, S) \
		/* Initializes each component of the vec<S> with the S arguments of POD type*/ \
		T##S(REPEAT_WITH_ID_AND_COMMA(plane_types::##T f, S)) : basevar(Type::variable) \
		{ \
			REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##T, f, S); \
			src->optype = node::OpType::o##T; \
		};

#define class_vec_def_size1(T) \
	/* All vectors of size 1: vec1, ivec1, uvec1, bvec1, dvec1*/ \
	class T##1: public basevar<node::DataType::##T, node::DataSize(1)>{ \
	public: \
		/* Default constructor */ \
		T##1(Type t) : basevar(t) {}; \
		/* Constructor from one argument of POD type*/ \
		T##1(plane_types::##T f0) : basevar(Type::variable) { src->data[0].d_##T = f0; src->optype = node::OpType::o##T; }; \
	};

#define class_vec_def_size2(T) \
	/* All vectors of size 2: vec2, ivec2, uvec2, bvec2, dvec2*/ \
	class T##2: public basevar<node::DataType::##T, node::DataSize(2)>{ \
	public: \
		/* Default constructor*/ \
		T##2(Type t) : basevar(t) {}; \
		cast_from_scalar(T, 2); \
		main_constructor(T, 2); \
		cast_from_const_literal_scalar(T, 2); \
	};

#define class_vec_def_size3(T) \
	/* All vectors of size 3: vec3, ivec3, uvec3, bvec3, dvec3*/ \
	class T##3: public basevar<node::DataType::##T, node::DataSize(3)>{ \
	public: \
		/*Default constructor*/ \
		T##3(Type t) : basevar(t) {}; \
		cast_from_scalar(T, 3); \
		main_constructor(T, 3); \
		cast_from_const_literal_scalar(T, 3); \
	};

#define class_vec_def_size4(T) \
	/* All vectors of size 4: vec4, ivec4, uvec4, bvec4, dvec4*/ \
	class T##4: public basevar<node::DataType::##T, node::DataSize(4)>{ \
	public: \
		/*Default constructor*/ \
		T##4(Type t) : basevar(t) {}; \
		cast_from_scalar(T, 4); \
		main_constructor(T, 4); \
		cast_from_const_literal_scalar(T, 4); \
	};

#define class_mat_def_(T, PT, M, N, MbyN) \
	class T##M##x##N: public basevar<node::DataType::##T, node::DataSize(M), node::DataSize(N)>{ \
	public: \
		T##M##x##N(Type t) : basevar(t) {}; \
		T##M##x##N(plane_types::##PT f) : basevar(Type::variable) { REPEAT_ASSIGNMENT(src->data[(N + 1) *, ].d_##PT, f, M); src->optype = node::OpType::o##PT; }; \
		T##M##x##N(REPEAT_WITH_ID_AND_COMMA(plane_types::##PT f, MbyN)) : basevar(Type::variable) { REPEAT_ASSIGNMENT_WITH_ID(src->data[, ].d_##PT, f, MbyN); src->optype = node::OpType::o##PT; }; \
	};
#define class_mat_def(T, PT, M, N) class_mat_def_(T, PT, M, N, MULL(M, N))

#define class_vec_def(T) \
	class_vec_def_size1(T) \
	class_vec_def_size2(T) \
	class_vec_def_size3(T) \
	class_vec_def_size4(T)

	class_vec_def(vec);
	class_vec_def(ivec);
	class_vec_def(uvec);
	class_vec_def(bvec);

	class_mat_def(mat, vec, 2, 2);
	class_mat_def(mat, vec, 3, 3);
	class_mat_def(mat, vec, 4, 4);

	typedef mat2x2 mat2;
	typedef mat3x3 mat3;
	typedef mat4x4 mat4;

#define op_ariphm_allvecTypes(S1, S2, S3) \
	op_ariphm(vec##S1, vec##S2, vec##S3) \
	op_ariphm(bvec##S1, bvec##S2, bvec##S3) \
	op_ariphm(ivec##S1, ivec##S2, ivec##S3) \
	op_ariphm(uvec##S1, uvec##S2, uvec##S3)
	
	op_ariphm_allvecTypes(1, 1, 1);
	op_ariphm_allvecTypes(2, 2, 2);
	op_ariphm_allvecTypes(3, 3, 3);
	op_ariphm_allvecTypes(4, 4, 4);

	// One operand is a scalar, and the other is a vector or matrix. In this case, the scalar operation is 
	// applied independently to each component of the vector or matrix, resulting in the same size vector
	// or matrix
	op_ariphm_allvecTypes(1, 2, 2);
	op_ariphm_allvecTypes(1, 3, 3);
	op_ariphm_allvecTypes(1, 4, 4);

	op_ariphm_allvecTypes(2, 1, 2);
	op_ariphm_allvecTypes(3, 1, 2);
	op_ariphm_allvecTypes(4, 1, 3);

	//op_ariphm(mat2, mat2, mat2);
	//op_ariphm(mat2, mat2, mat2);
	
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
				ss << n->data[0].d_vec;
			}
			break;
			case node::integerConstant:
			{
				ss << n->data[0].d_ivec;
			}
			break;
			case node::unsignedIntegerConstant:
			{
				ss << n->data[0].d_uvec << "u";
			}
			break;
			case node::booleanConstant:
			{
				if (n->data[0].d_bvec)
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
			case node::cast:
			{
				ss << GetType(n) << "(";
				ss << GetId(n->childs[0]);
				for (int i = 1; i < int(n->childs.size()); ++i)
				{
					ss << ", " << GetId(n->childs[0]);
				}
				ss << ")";
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

			std::string size_secondary;
			switch (n->datasize_secondary)
			{
			case node::DataSize::size1: size_secondary = "1"; break;
			case node::DataSize::size2: size_secondary = "2"; break;
			case node::DataSize::size3: size_secondary = "3"; break;
			case node::DataSize::size4: size_secondary = "4"; break;
			}

			switch (n->datatype)
			{
			case node::DataType::bvec: return "bvec" + size;
			case node::DataType::ivec: return "ivec" + size;
			case node::DataType::uvec: return "uvec" + size;
			case node::DataType::dvec: return "dvec" + size;
			case node::DataType::vec: return "vec" + size;

			case node::DataType::mat: return "mat" + size + "x" + size_secondary;
			case node::DataType::dmat: return "dmat" + size + "x" + size_secondary;
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

	template<typename T>
	inline std::string genShader(T v)
	{
		context ctx;
		ctx.VisitNode(v.src);
		return ctx.GenerateCode();
	}
}
