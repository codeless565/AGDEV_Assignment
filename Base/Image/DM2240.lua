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

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2-x1)*(x2-x1) +(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1)
	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minimum = 9999999
	local maximum = -9999999
	local n = arg.n
	local avg = 0

	for i,v in ipairs(arg) do
		if v < minimum then
			minimum = v
		end
		
		if v > maximum then
			maximum = v
		end
		
		avg = avg + v
	end 
	avg = avg / n
	
	print(minimum,maximum,avg,n)
	return minimum,maximum,avg,n
end

title = "DM2240 - Week 14 Scripting"
width =  800
height = 600
test = false -- Not in use for anything, placeholder 

-- Keyboard Inputs
moveForward = "W"
moveBackward = "S"
moveLeft = "A"
moveRight = "D"

-- CPlayerInfo start position
CPlayerInfoStartPos = {0,0,10}