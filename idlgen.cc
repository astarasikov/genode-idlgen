#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <idlgen.hh>

struct SessionGenerator : public Generator {
	const std::string _name;

	SessionGenerator(const std::string name)
		: Generator(), _name(name)
	{}
	virtual void generate(Interface *iface) const {
		std::cout << "struct Session_client : Genode::Rpc_client<Session>" 
		<< std::endl;
		std::cout << "{" << std::endl;

		for (Method *m : iface->methods()) {
			std::cout << "\t";
			std::cout << m->returnType()->name();
			std::cout << " " << m->name();
			
			// argument list
			std::cout << "(";
			bool firstArg = true;
			for (auto &kv : m->arguments()) {
				if (!firstArg) {
					std::cout << ", ";
				}
				else {
					firstArg = false;
				}

				std::cout << kv.second->name() << " ";
				std::cout << kv.first;
			}
			std::cout << ")";
			
			//RPC stub body
			std::cout << " {" << std::endl;
			std::cout << "\t\tcall<Rpc_" << m->name() << ">(";

			bool firstRpcArg = true;
			for (auto &kv : m->arguments()) {
				if (!firstRpcArg) {
					std::cout << ", ";
				}
				else {
					firstRpcArg = false;
				}

				std::cout << kv.first;
			}

			std::cout << ");" << std::endl;
			std::cout << "\t}" << std::endl;
		}

		std::cout << "};" << std::endl;
	}
};

static void test_gpio(void) {
	struct Method_write : public Method {
		ArgMap _argMap;

		Method_write() {
			_argMap["value"] = builtinType(BOOL);
		}

		virtual const std::string name(void) const {
			return "write";
		}
		virtual Type *returnType(void) const {
			return builtinType(VOID);
		}
		virtual const ArgMap &arguments(void) const {
			return _argMap;
		}
	};

	struct Iface_gpio : public Interface {
		MethodList _methodList;
		Method_write method_write;

		Iface_gpio() {
			_methodList.push_back(&method_write);
		}

		virtual const MethodList &methods(void) const {
			return _methodList;
		}
	} iface_gpio;

	SessionGenerator g("Gpio");
	g.generate(&iface_gpio);
};

int main(int argc, char* argv[]) {
	test_gpio();
	return 0;
}
