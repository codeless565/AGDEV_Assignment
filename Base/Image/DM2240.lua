function SaveToLuaFile(outputString, overwrite)
	print("SavingToLuaFile...")
	local f; -- The File
	
	if overwrite == 1 then 
		f = assert(io.open("Image/DM2240_HighScore.lua","w"))
	elseif overwrite == 0 then 								-- Append with new data
		f = assert(io.open("Image/DM2240_HighScore.lua","a"))
	end
	-- Write to file
	f:write(outputString)
	-- Close the file
	f:close()
	print("OK")
end

title = "DM2240 - Week 14 Scripting"
width =  800
height = 600
test = false -- Not in use for anything, placeholder 
