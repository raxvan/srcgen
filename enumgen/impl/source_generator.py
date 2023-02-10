
import importlib.util
import cpp_generator

class EnumBuilder():
	def __init__(self, name):

		self._name = name

		self.keyvalues = {} #key alias
		self.keylength = {}
		self.aliaslength = {}

		self.enumPrefix = "k"
		self.aliasPtrfix = ""

		self.namespace = None
		self.classname = None
		self.output = None


		self._max_alias_len = 0
		self._min_alias_len = 100000

		self._max_name_len = 0
		self._min_name_len = 100000

	def add_internal(self, name, alias):
		self.keyvalues.setdefault(name, set()).add(alias)

		nlen = len(name)
		if nlen > self._max_name_len:
			self._max_name_len = nlen
		elif nlen < self._min_name_len:
			self._min_name_len = nlen

		alen = len(alias)
		if alen > self._max_alias_len:
			self._max_alias_len = alen
		elif alen < self._min_alias_len:
			self._min_alias_len = alen

		self.keylength.setdefault(nlen, []).append(name)
		self.aliaslength.setdefault(alen, []).append((name,alias))

	def map_to_index(self):
		index = 0
		result = []
		sk = sorted([k for k,_ in self.keyvalues.items()], key = lambda x: (len(x), x))
		for s in sk:
			result.append((s,index))
			index = index + 1

		return result

	def add(self, name, alias = None):
		if alias != None:
			self.add_internal(name, alias)
		else:
			self.add_internal(name, name)


def _load_module(abs_path_to_pyfile, load_location):
	ll = "enumpak." + load_location
	spec = importlib.util.spec_from_file_location(ll, abs_path_to_pyfile)
	module_context = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(module_context)

	return module_context

def generate(name, inputenum, baseout):
	ctx = _load_module(inputenum, name)

	if hasattr(ctx, "make_enum") != False:
		eb = EnumBuilder(name)
		make_enum_func = getattr(ctx,"make_enum")
		make_enum_func(eb)
		cpp_generator.build(eb, baseout)


