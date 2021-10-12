import requests
 
# Making a PUT request
r = requests.put('http://0.0.0.0', data ={'key':'value'})
r1 = requests.get('http://0.0.0.0:80', data ={'key':'value'})
r2 = requests.get('http://0.0.0.0:90', stream=True)
r3 = requests.put('http://0.0.0.0:100', data ={'key':'value'})
r4 = requests.put('http://0.0.0.0', data ={'key':'value'})

# check status code for response received
# success code - 200
print(r)
print(r.content)

print(r1)
print(r1.content)

print(r2)
print(r2.content)

print(r3)
print(r3.content)

print(r4)
print(r4.content)
 
# print content of request