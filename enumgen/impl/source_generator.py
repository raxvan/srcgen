
import sys
import importlib.util

################################################################################################################################################################

class EnumDesc():
	def __init__(self, _name):

		self.name = _name
		self.keys = {} # key -> data
		self.order = []

		self._max_key_length = 0
		self._min_key_length = sys.maxsize

	def add(self, name, **kwargs):
		if name in self.keys:
			raise Exception(f"Duplicate enum key {name}!")
		self.keys[name] = kwargs
		self.order.append((name, len(self.order)))

		kl = len(name)
		self._max_key_length = max(self._max_key_length, kl)
		self._min_key_length = min(self._min_key_length, kl)

	def join(self, e):

		for k, i in e.order:

			if k in self.keys:
				raise Exception(f"Duplicate enum key {k}!")

			self.keys[k] = e.keys[k]

			kl = len(k)

			self.order.append((k, len(self.order)))
			self._max_key_length = max(self._max_key_length, kl)
			self._min_key_length = min(self._min_key_length, kl)


################################################################################################################################################################

class EnumConstructor():
	def __init__(self, e):
		self.enum = e

	def build_sorted_order(self):
		sorted_keys = sorted(self.enum.keys, key = lambda x: (len(x), x))
		index = 0
		result = []
		for k in sorted_keys:
			result.append((k, index))
			index = index + 1

		self.enum.order = result
		return result

	def get_names(self):
		return self.enum.keys.keys()

	def get_property(self, name, *args):
		v = self.enum.keys[name]
		for a in args:
			r = v.get(a,None)
			if r != None:
				return r

		return v

################################################################################################################################################################

def _load_module(abs_path_to_pyfile, load_location):
	ll = "enumpak." + load_location
	spec = importlib.util.spec_from_file_location(ll, abs_path_to_pyfile)
	module_context = importlib.util.module_from_spec(spec)
	spec.loader.exec_module(module_context)
	return module_context

def generate(name, module, baseout):
	ctx = _load_module(module, name)

	evalue = EnumDesc(name)

	if hasattr(ctx, "construct"):
		construct_func = getattr(ctx,"construct")
		construct_func(evalue)

	if hasattr(ctx, "generate_cpp_enum"):
		import cpp_generator
		generate_func = getattr(ctx,"generate_cpp_enum")

		ctx = cpp_generator.EnumBuilder(EnumConstructor(evalue), baseout)
		generate_func(ctx)
		ctx.build()



