
import os

def write_to_file(content, file):
	f = open(file, "w")
	f.write(content)
	f.close()

def append(line, depth, content):
	return line + "\t" * depth + content + "\n"

################################################################################################
################################################################################################
class Generator():
	def __init__(self, builder, outdir):
		self.output = os.path.join(outdir, builder.output)
		
		if not os.path.exists(self.output):
			os.makedirs(self.output)

		self.data = builder

		self.index_list = builder.map_to_index()

	def build_header_struct(self, depth):
		s = ""

		#start struct:
		s = append(s, depth, f"struct {self.data.classname}")
		s = append(s, depth, "{")

		#decl
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, "enum Enum")
		s = append(s, depth + 1, "{")

		maxlen = self.data._max_name_len + len(self.data.cppPrefix) + 1
		for k,i in self.index_list:
			s = append(s, depth + 2, (self.data.cppPrefix + k).ljust(maxlen) + f"= {i},")

		s = append(s, depth + 2, ("kCount").ljust(maxlen) + f"= {len(self.index_list)}")

		s = append(s, depth + 1, "};")

		#functions
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, f"static {self.data.classname}::Enum parse(const char* name, const std::size_t nmsize);")
		s = append(s, depth + 1, f"static std::string_view name(const {self.data.classname}::Enum v);")

		#static functions:
		s = append(s, depth + 0, "public:")
		s = append(s, depth + 1, "constexpr static uint32_t count()")
		s = append(s, depth + 1, "{")
		s = append(s, depth + 2, f"return {len(self.index_list)};")
		s = append(s, depth + 1, "}")

		#end struct:

		s = append(s, depth, "};")

		return s

	def build_header_impl(self):
		if self.data.namespace != None:
			s = ""
			s = append(s, 0, f"#pragma once")
			s = append(s, 0, f"#include <generated_source.h>")
			s = append(s, 0, f"namespace {self.data.namespace}")
			s = append(s, 0, "{")
			s = s + self.build_header_struct(1)
			s = append(s, 0, "}")
			return s
		else:
			return self.build_header_struct(0)

	def build_header(self):
		h = self.build_header_impl()
		write_to_file(h, os.path.join(self.output, self.data._name + '_decl.h'))

	################################################################################################
	################################################################################################

	def build_implementation(self, depth):
		s = ""
		s = append(s, depth, f"{self.data.classname}::Enum {self.data.classname}::parse(const char* _name, const std::size_t nmsize)")
		s = append(s, depth, "{")
		prefixlen = len(self.data.strPtrfix)
		s = append(s, depth + 1, f"if (_name == nullptr)")
		s = append(s, depth + 2, f"return {self.data.classname}::Enum::kCount;")

		s = append(s, depth + 1, f"switch (nmsize)")
		s = append(s, depth + 1, "{")
		alengths = sorted(self.data.aliaslength.keys())
		for ln in alengths:
			lnlist = self.data.aliaslength[ln]
			s = append(s, depth + 2, f"case {prefixlen + ln}:")
			s = append(s, depth + 2, "{")
			for (kname, aname) in lnlist:
				s = append(s, depth + 3, f'if (std::strncmp(_name, "{self.data.strPtrfix}{aname}", {prefixlen + ln}) == 0)')
				s = append(s, depth + 4, f'return Enum::{self.data.cppPrefix}{kname};')
			s = append(s, depth + 3, f'break;')

			s = append(s, depth + 2, "}")
		s = append(s, depth + 2, "default:break;")
		s = append(s, depth + 1, "}")
		s = append(s, depth + 1, f"return {self.data.classname}::Enum::kCount;")

		s = append(s, depth, "}")
		s = append(s, depth, f"std::string_view {self.data.classname}::name(const {self.data.classname}::Enum v)")
		s = append(s, depth, "{")
		s = append(s, depth + 1, "switch(v)")
		s = append(s, depth + 1, "{")

		maxlen = self.data._max_name_len + len(self.data.cppPrefix) + 1
		
		for k,i in self.index_list:
			nmKey = f"{self.data.cppPrefix}{k}"
			nmAlias = f"{self.data.strPtrfix}"
			for f in self.data.keyvalues[k]:
				nmAlias = nmAlias + f
				break

			cs = f'case Enum::' + nmKey.ljust(maxlen) + f': return std::string_view("{nmAlias}", {len(nmAlias)});'
			s = append(s, depth + 2, cs)

		s = append(s, depth + 2, "default:break;")
		s = append(s, depth + 1, "}")
		s = append(s, depth + 1, "return {};")

		s = append(s, depth, "}")

		return s

	def build_source(self):
		s = ""

		hname = self.data._name + '_decl.h'
		s = append(s, 0, f'#include "{hname}"')
		if self.data.namespace != None:
			s = append(s, 0, f"namespace {self.data.namespace}")
			s = append(s, 0, "{")
			s = s + self.build_implementation(1)
			s = append(s, 0, "}")
		else:
			s = s + self.build_implementation(0)

		write_to_file(s, os.path.join(self.output, self.data._name + '_impl.h'))

################################################################################################
################################################################################################

def build(builder, outdir):
	b = Generator(builder, outdir)
	b.build_header()
	b.build_source()
	



