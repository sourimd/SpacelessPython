def f(n):
return n
end
def g():
a = 1
c = 1
b = f(c)
a = a * b
c = c+1
b = f(c)
a = a * b
c = c+1
b = f(c)
a = a * b
c=c+1
b = f(c)
a = a * b
return a
end
def main():
x = g()
print x
end
