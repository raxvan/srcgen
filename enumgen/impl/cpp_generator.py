
import os
import numpy

def write_to_file(content, file):
	f = open(file, "w")
	f.write(content)
	f.close()
	print(f"AUTOGENERATED:{file}")

def append(line, depth, content):
	return line + "\t" * depth + content + "\n"

def simple_string_hash(s):
	hash_value = numpy.uint32(1423)

	for c in s:
		hash_value = (hash_value << numpy.uint32(4)) + numpy.uint32(ord(c))

	return hash_value

################################################################################################
################################################################################################

class EnumBuilder():
	def __init__(self, ec):
		self.enumPrefix = "k"
		self.aliasPrefix = ""

		self.namespace = None
		self.classname = None
		self.output = None


		self._module = ec.enum
		self._build = ec

	def build(self, refmodules, baseout):
		outdir = os.path.join(baseout, self.output)

		if not os.path.exists(outdir):
			os.makedirs(outdir)

		sources = [os.path.relpath(r, outdir) for r in refmodules]

		self.build_header(outdir, sources)
		self.build_source(outdir, sources)

	def build_header(self, outdir, sources):
		h = self.build_header_impl(sources)
		write_to_file(h, os.path.join(outdir, self._module.name + '_decl.h'))

	def build_source(self, outdir, sources):
		s = self.build_source_impl(sources)
		write_to_file(s, os.path.join(outdir, self._module.name + '_impl.h'))

	def build_header_struct(self, depth):

		index_order = self._build.build_sorted_order()

		s = ""

		#start struct:
		s = append(s, depth, f"struct {self.classname}")
		s = append(s, depth, "{")

		#decl
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, "enum Enum")
		s = append(s, depth + 1, "{")

		maxlen = self._module._max_key_length + len(self.enumPrefix) + 1

		for k,i in index_order:
			s = append(s, depth + 2, (self.enumPrefix + k).ljust(maxlen) + f"= {i},")

		s = append(s, depth + 2, ("kCount").ljust(maxlen) + f"= {len(index_order)}")

		s = append(s, depth + 1, "};")

		#functions
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, f"static {self.classname}::Enum parse(const char* name, const std::size_t nmsize);")
		s = append(s, depth + 1, f"static std::string_view name(const {self.classname}::Enum v);")

		#static functions:
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, "constexpr static uint32_t count()")
		s = append(s, depth + 1, "{")
		s = append(s, depth + 2, f"return {len(index_order)};")
		s = append(s, depth + 1, "}")

		#visit function
		#s = append(s, depth + 1, "template <class F>")
		#s = append(s, depth + 1, "static void visit(const F& _func)")
		#s = append(s, depth + 1, "{")
		#for k,i in index_order:
		#	s = append(s, depth + 2, f"_func({self.enumPrefix + k});")
		#s = append(s, depth + 1, "}")

		#end struct:
		s = append(s, depth, "};")

		return s

	def build_header_impl(self, sources):
		if self.namespace != None:
			s = ""
			s = append(s, 0, f"#pragma once")
			s = append(s, 0, f"/*")
			s = append(s, 0, f"Autogenerated from:")
			for src_path in sources:
				s = append(s, 1, f"{src_path}")
			s = append(s, 0, f"*/")
			s = append(s, 0, f"#include <generated_source.h>")
			s = append(s, 0, f"namespace {self.namespace}")
			s = append(s, 0, "{")
			s = s + self.build_header_struct(1)
			s = append(s, 0, "}")
			return s
		else:
			return self.build_header_struct(0)

	def build_source_impl(self, sources):
		s = ""

		hname = self._module.name + '_decl.h'
		s = append(s, 0, f"/*")
		s = append(s, 0, f"Autogenerated from:")
		for src_path in sources:
			s = append(s, 1, f"{src_path}")
		s = append(s, 0, f"*/")
		s = append(s, 0, f'#include "{hname}"')
		if self.namespace != None:
			s = append(s, 0, f"namespace {self.namespace}")
			s = append(s, 0, "{")
			s = s + self.build_implementation(1)
			s = append(s, 0, "}")
		else:
			s = s + self.build_implementation(0)

		return s

	def _get_alias(self, name):
		p = self._build.get_property(name, "alias")
		if isinstance(p, str):
			return p

		return name

	def _make_alias(self, name):
		a = self._get_alias(name);
		return (name, f"{self.aliasPrefix}{a}")

	def build_implementation(self, depth):

		aliases = {n : self._get_alias(n) for n in self._build.get_names()}
		lengths = {}
		lenorder = set()
		for n,a in aliases.items():
			lenorder.add(len(a))
			lengths.setdefault(len(a),[]).append((n,a))
		lenorder = sorted(list(lenorder))

		prefixlen = len(self.aliasPrefix)

		s = ""
		s = append(s, depth, f"{self.classname}::Enum {self.classname}::parse(const char* _name, const std::size_t nmsize)")
		s = append(s, depth, "{")
		s = append(s, depth + 1, f"if (_name == nullptr || nmsize <= {prefixlen})")
		s = append(s, depth + 2, f"return {self.classname}::Enum::kCount;")

		s = append(s, depth + 1, f'if(std::strncmp(_name, "{self.aliasPrefix}", {prefixlen}) != 0)')
		s = append(s, depth + 2, f"return {self.classname}::Enum::kCount;")
		s = append(s, depth + 1, "const char* test_str = _name + 6;")
		s = append(s, depth + 1, "std::size_t test_size = nmsize - 6;")
		s = append(s, depth + 1, "uint32_t name_hash = gs::utils::simple_string_hash(test_str, test_size);")
		s = append(s, depth + 1, "auto test_alias = [=](const char* eref) -> bool {")
		s = append(s, depth + 2, f"return std::strncmp(eref + {prefixlen}, test_str, test_size) == 0;")
		s = append(s, depth + 1, "};")


		s = append(s, depth + 1, f"switch (nmsize)")
		s = append(s, depth + 1, "{")



		for current_size in lenorder:
			case_size = current_size + prefixlen
			s = append(s, depth + 2, f"case {case_size}:")
			s = append(s, depth + 2, "{")

			items_with_this_length = lengths[current_size]

			for kname, aname in items_with_this_length:
				s = append(s, depth + 3, f'if (name_hash == {simple_string_hash(aname)} && test_alias("{self.aliasPrefix}{aname}"))')
				s = append(s, depth + 4, f'return Enum::{self.enumPrefix}{kname};')

			s = append(s, depth + 3, f'break;')
			s = append(s, depth + 2, "}")

		s = append(s, depth + 2, "default:break;")
		s = append(s, depth + 1, "}")
		s = append(s, depth + 1, f"return {self.classname}::Enum::kCount;")

		s = append(s, depth, "}")


		s = append(s, depth, f"std::string_view {self.classname}::name(const {self.classname}::Enum v)")
		s = append(s, depth, "{")
		s = append(s, depth + 1, "switch(v)")
		s = append(s, depth + 1, "{")

		maxlen = self._module._max_key_length + len(self.enumPrefix) + 1

		for k, i in self._module.order:
			nmKey = f"{self.enumPrefix}{k}"
			nmAlias = f"{self.aliasPrefix}{aliases[k]}"

			cs = f'case Enum::' + nmKey.ljust(maxlen) + f': return std::string_view("{nmAlias}", {len(nmAlias)});'
			s = append(s, depth + 2, cs)

		s = append(s, depth + 2, "default:break;")
		s = append(s, depth + 1, "}")
		s = append(s, depth + 1, "return {};")

		s = append(s, depth, "}")

		return s






