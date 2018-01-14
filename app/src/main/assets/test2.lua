local myfile = io.open("/storage/emulated/0/AutomaticKey/2018-01-13", "r")
if nil == myfile then
    print("open file for dir fail")
end
print("\n======commond dir result:")
for cnt in myfile:lines() do
    print(cnt)
end
myfile:close()



os.execute("input keyevent 4")

local secondfile = io.popen("input keyevent 4")
if nil == secondfile then
    print("open file for ipconfig fail")
end
print("\n======commond ipconfig result:")
local content = secondfile:read("*a")
print(content)
secondfile:close()

for file in io.popen("ls /mnt/"):lines() do
    print(file)
end