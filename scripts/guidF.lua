require "string_utils" ()

--[[
	Transforms:
		16b45da2-7d70-4aea-a58d-760e9ecb841d
	To:
		{ 0x16b45da2, 0x7d70, 0x4aea, { 0xa5, 0x8d, 0x76, 0x0e, 0x9e, 0xcb, 0x84, 0x1d}}
		
	Input:
		"16b45da2-7d70-4aea-a58d-760e9ecb841d"
]]
function guidtr(str)
	local result = "{ 0x"
	local t = str:splitBy("[^%-]+")

	if #t ~= 5 then
		error("Not 4 '-' chars were found in input.\n\tInput: "..tostring(str))
	end
	
	if #t[1] ~= 8 then 
		error("First number is incorrect.\n\tInput: "..tostring(t[1]))
	end
	if #t[2] ~= 4 then 
		error("Second number is incorrect.\n\tInput: "..tostring(t[2]))
	end
	if #t[3] ~= 4 then 
		error("Third number is incorrect.\n\tInput: "..tostring(t[3]))
	end
	if #t[4] ~= 4 then 
		error("Fourth number is incorrect.\n\tInput: "..tostring(t[4]))
	end
	if #t[5] ~= 12 then 
		error("Fifth number is incorrect.\n\tInput: "..tostring(t[5]))
	end
	
	result = result:append(t[1])
	result = result:append(", 0x")
	
	result = result:append(t[2])
	result = result:append(", 0x")
	
	result = result:append(t[3])
	result = result:append(", { 0x")
	
	result = result:append(t[4]:sub(1,2))
	result = result:append(", 0x")
	result = result:append(t[4]:sub(3,4))
	
	for i = 0,5 do
		result = result:append(", 0x")
		result = result:append(t[5]:sub(1 + i*2,2 + i*2))
	end
	result = result:append("}}")
	
	print(result)
	return result
end

--[[
	Transforms:
		{ 0x16b45da2, 0x7d70, 0x4aea, { 0xa5, 0x8d, 0x76, 0x0e, 0x9e, 0xcb, 0x84, 0x1d}}
	To:
		16B45DA2-7D70-4AEA-A58D-760E9ECB841D
		
	Input:
		"{ 0x16b45da2, 0x7d70, 0x4aea, { 0xa5, 0x8d, 0x76, 0x0e, 0x9e, 0xcb, 0x84, 0x1d}}"
]]
function guidRevTr(str)
	local t = str:splitBy("0x(%x+)")
	
	if #t ~= 11 then
		error("Not 11 numbers were found in input.\n\tInput: "..tostring(str))
	end
	
	if #t[1] ~= 8 then 
		error("1-st number is incorrect.\n\tInput: "..tostring(t[1]))
	end
	if #t[2] ~= 4 then 
		error("2-nd number is incorrect.\n\tInput: "..tostring(t[2]))
	end
	if #t[3] ~= 4 then 
		error("3-rd number is incorrect.\n\tInput: "..tostring(t[3]))
	end
	if #t[4] ~= 2 then 
		error("4-th number is incorrect.\n\tInput: "..tostring(t[4]))
	end
	if #t[5] ~= 2 then 
		error("5-th number is incorrect.\n\tInput: "..tostring(t[5]))
	end
	if #t[6] ~= 2 then 
		error("6-th number is incorrect.\n\tInput: "..tostring(t[6]))
	end
	if #t[7] ~= 2 then 
		error("7-th number is incorrect.\n\tInput: "..tostring(t[7]))
	end
	if #t[8] ~= 2 then 
		error("8-th number is incorrect.\n\tInput: "..tostring(t[8]))
	end
	if #t[9] ~= 2 then 
		error("9-th number is incorrect.\n\tInput: "..tostring(t[9]))
	end
	if #t[10] ~= 2 then 
		error("10-th number is incorrect.\n\tInput: "..tostring(t[10]))
	end
	if #t[11] ~= 2 then 
		error("11-th number is incorrect.\n\tInput: "..tostring(t[11]))
	end
	local out_ = t[1].."-"..t[2].."-"..t[3].."-"..t[4]..t[5].."-"
	for i = 6, 11 do out_ = out_:append(t[i]) end
	print(out_:upper())
	return out_:upper()
end