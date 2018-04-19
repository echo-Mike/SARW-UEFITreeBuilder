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

