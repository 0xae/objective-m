[Runtime def:hi,(arg),{[arg hi:arg]}]


[Runtime def:iter,(this,L),{[[(1,2,3) head] println];[@"hi" println]}]
[Runtime iter:Runtime,(1,2,3)]

