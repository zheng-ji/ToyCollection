#!/usr/bin/env lua

function byteToUint32(a,b,c,d)
    local _int = 0
    if a then
        _int = _int +  bit32.lshift(a, 24)
    end
    _int = _int + bit32.lshift(b, 16)
    _int = _int + bit32.lshift(c, 8)
    _int = _int + d
    if _int >= 0 then
        return _int
    else
        return _int + math.pow(2, 32)
    end
end

local phone_dat = "phone.dat"

function getPhoneType(a)
    if a == 1 then
        return "移动"
    elseif a == 2 then
        return "联通"
    elseif a==3 then
        return "电信"
    elseif a==4  then
        return "电信虚拟运营商"
    elseif a==5 then
        return "联通虚拟运营商"
    elseif a ==6 then
        return "移动虚拟运营商"
    else
        return ""
    end
end

local file = io.open(phone_dat)
if file == nil then
    buf = nil
else
    buf = file:read("*all")
end

function phoneFind(phone)

    if buf == nil then
        print("buf is nil")
        return
    end
    local phone = string.sub(phone, 0, 7)
    local int_phone = tonumber(phone)
    --> print(phone)
    --> print("buf length:",string.len(buf))
    local version = string.sub(buf,1,4)
    --> print("version:",version)
    local firstIndexOffset = byteToUint32(
    string.byte(buf,8),
    string.byte(buf,7),
    string.byte(buf,6),
    string.byte(buf,5)
    )
    --> print("first index offset:",firstIndexOffset)
    local phoneRecordCount = math.floor((string.len(buf) - firstIndexOffset) / 9)
    local phoneIndexLength = 9
    -->print("phone record count:", phoneRecordCount)
    local left = 0
    local right = phoneRecordCount
    while (left <= right) do
        local middle = math.floor((left + right ) / 2)
        local curOffset = firstIndexOffset + middle * phoneIndexLength
        if curOffset >= string.len(buf) then
            return
        end
        local indexRecord = string.sub(buf, curOffset+1, curOffset + 9)
        local curPhone = byteToUint32(
        string.byte(indexRecord,4), 
        string.byte(indexRecord,3), 
        string.byte(indexRecord,2), 
        string.byte(indexRecord,1)  
        )
        local phone_type = string.byte(indexRecord, 9)
        --> print(curPhone,phone_type)
        if curPhone > int_phone then
            right = middle - 1
        elseif curPhone < int_phone then
            left = middle + 1
        else 
            local dataOffset = byteToUint32(
            string.byte(indexRecord,8), 
            string.byte(indexRecord,7), 
            string.byte(indexRecord,6), 
            string.byte(indexRecord,5)
            )
            --> print(dataOffset)
            local tmp = string.sub(buf, dataOffset + 1, dataOffset + 1024)
            local t = string.find(tmp, '\0')
            if t then
                return string.format("%s%s%s",string.sub(tmp, 1, t-1), "|" ,getPhoneType(phone_type))
            end
            break
        end
    end
end

print(phoneFind("15920554608"))
