
system.init()
print(os.time())
system.sleep(1)
print(w)
print(h)
print(os.time())
system.click(450, 465)
system.sleep(2)
system.volumeDown()
system.inputText("你好")
system.back()
system.sleep(1)
system.home()
system.sleep(1)
system.menu()
system.sleep(1)
system.homePage()
system.sleep(1)

system.init()
w,h=system.getScreenSize()
downX = w/2
downY = y/4
system.init()
system.touchDown(downX, downY)

system.runApp("com.tencent.now/.app.startup.LauncherActivity")
system.close()

print(os.time())

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

