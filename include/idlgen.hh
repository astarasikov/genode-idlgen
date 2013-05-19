#ifndef __IDLNEN_HH__
#define __IDLNEN_HH__

#include <string>
#include <map>
#include <vector>

struct Type {
	virtual bool isPrimitive(void) const = 0;
	virtual const std::string name(void) const = 0;
};

struct Method {
	typedef std::map<std::string, Type*> ArgMap;

	virtual const std::string name(void) const = 0;
	virtual Type *returnType(void) const = 0;
	virtual const ArgMap &arguments(void) const = 0;
};

struct Interface {
	typedef std::vector<Method *> MethodList;

	virtual const MethodList &methods(void) const = 0;
};

struct Generator {
	explicit Generator() {}
	virtual void generate(Interface *desc) const = 0;
};

enum BuiltinType {
	VOID,
	BOOL,
	INT_8,
	INT_16,
	INT_32,
	INT_64,
	UINT_8,
	UINT_16,
	UINT_32,
	UINT_64,
	SIZE_T,
};

static inline std::map<enum BuiltinType, Type*> makeBuitins(void) {
	static std::map<enum BuiltinType, Type*> builtins;
	#define MK_BUILTIN(t, str) do {\
		static struct __type_t : public Type {\
			virtual bool isPrimitive(void) const {\
				return true;\
			}\
			virtual const std::string name(void) const {\
				return str;\
			}\
		} __type;\
		builtins[t] = &__type;\
	} while (0)
	#define MK_INT(sz) MK_BUILTIN(INT_ ##sz, "int_" #sz "t");
	#define MK_UINT(sz) MK_BUILTIN(INT_ ##sz, "uint_" #sz "t");

	MK_BUILTIN(VOID, "void");
	MK_BUILTIN(BOOL, "bool");
	MK_BUILTIN(SIZE_T, "size_t");
	MK_INT(8);
	MK_INT(16);
	MK_INT(32);
	MK_INT(64);
	MK_UINT(8);
	MK_UINT(16);
	MK_UINT(32);
	MK_UINT(64);

	#undef MK_UINT
	#undef MK_INT
	#undef MK_BUILTIN
	return builtins;
}

static Type *builtinType(enum BuiltinType type) {
	static std::map<enum BuiltinType, Type*> builtins = makeBuitins();
	return builtins[type];
}

#endif //__IDLNEN_HH__
