
class StdOut:
	def __init__(self, stdout, silent):
		self.stdout = stdout
		self.SupressOutput = silent

	def write(self, *data):
		if not self.SupressOutput:
			self.stdout.write(*data)

class RedirectClass:
	def __init__(self):
		self.redirect_stdout = None
		self.redirect_cout = None

	def Enable(self, silent):
		if self.redirect_stdout != None:
			raise Exception("Already redirected")

		self.redirect_stdout = StdOut(sys.stdout, silent)
		self.redirect_cout = core.redirect_cout()
		sys.stdout = self.redirect_stdout

	def Disable(self):
		if self.redirect_stdout == None:	
			raise Exception("Not redirected")

		core.restore_cout(self.redirect_cout)
		sys.stdout = sys.__stdout__

		self.redirect_stdout = None
		self.redirect_cout = None
		
	

Redirect = RedirectClass()
