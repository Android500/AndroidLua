print(os.time())
system.sleep(5000)
print(os.time())

print(os.time())
w,h=system.getScreenSize()
print(os.time())
print(w)
print(h)

local t= io.popen("/storage/emulated/0/AutomaticKey/2018-01-13")
local a = t:read("*all")
print(a)

local myfile = io.open("/storage/emulated/0/AutomaticKey", "r")
if nil == myfile then
    print("open file for dir fail")
end
print("\n======commond dir result:")
for cnt in myfile:lines() do
    print(cnt)
end
myfile:close()
local secondfile = io.popen("ipconfig")
if nil == secondfile then
    print("open file for ipconfig fail")
end
print("\n======commond ipconfig result:")
local content = secondfile:read("*a")
print(content)
secondfile:close()

