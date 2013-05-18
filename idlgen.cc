#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <idlgen.hh>

static std::string ToLower(const std::string &s) {
	std::string ret(s);
	std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
	return ret;
}

static std::string ToUpper(const std::string &s) {
	std::string ret(s);
	std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
	return ret;
}

struct IfdefGuard {
	const std::string &_name;
	std::ostream &_stream;

	IfdefGuard(const std::string &name, std::ostream &_stream)
		: _name(name), _stream(_stream)
	{
		_stream << "#ifndef " << _name << "\n";
		_stream << "#define " << _name << "\n\n";
	}

	~IfdefGuard() {
		_stream << "\n#endif // " << _name << "\n";
	}
};

struct SessionGenerator : public Generator {
	const std::string _name;
	std::ostream &_stream;

	SessionGenerator(const std::string name, std::ostream &_stream)
		: Generator(), _name(name), _stream(_stream)
	{}
	virtual void generate(Interface *iface) const {
		//ifdef guard
		auto hdr_name = "_INCLUDE__" + ToUpper(_name) + 
			"_SESSION_H__CLIENT_H_"; 
		IfdefGuard ig(hdr_name, _stream);

		_stream << "struct Session_client : Genode::Rpc_client<Session>\n";
		_stream << "{\n";

		for (Method *m : iface->methods()) {
			_stream << "\t";
			_stream << m->returnType()->name();
			_stream << " " << m->name();
			
			// argument list
			_stream << "(";
			bool firstArg = true;
			for (auto &kv : m->arguments()) {
				if (!firstArg) {
					_stream << ", ";
				}
				else {
					firstArg = false;
				}

				_stream << kv.second->name() << " ";
				_stream << kv.first;
			}
			_stream << ")";
			
			//RPC stub body
			_stream << " {\n";
			_stream << "\t\tcall<Rpc_" << m->name() << ">(";

			bool firstRpcArg = true;
			for (auto &kv : m->arguments()) {
				if (!firstRpcArg) {
					_stream << ", ";
				}
				else {
					firstRpcArg = false;
				}

				_stream << kv.first;
			}

			_stream << ");\n";
			_stream << "\t}\n";
		}

		_stream << "};\n";
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

	SessionGenerator g("Gpio", std::cout);
	g.generate(&iface_gpio);
};

int main(int argc, char* argv[]) {
	test_gpio();
	return 0;
}
