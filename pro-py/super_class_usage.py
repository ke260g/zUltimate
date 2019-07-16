class Parent:
	def __init__(self):
		# do some init
		pass

class Children(Parent):
	def __init__(self):
		super(Parent, self).__init__()
		# do more init
		pass