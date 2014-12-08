def g(n):
return n
end
def h(n):
v = g(1)
x = n * v
return x
end
def i(n):
t = h(2)
s = n * t
return s
end
def j(n):
p = i(3)
q = n * p
return q
end
def main():
a = j(4)
print a
end
