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

function SaveToOptionsFile(outputString, overwrite)
	local f; -- The File
	
	if overwrite == 1 then 
		f = assert(io.open("Image/Options2.lua","w"))
	elseif overwrite == 0 then 								-- Append with new data
		f = assert(io.open("Image/Options2.lua","a"))
	end
	-- Write to file
	f:write(outputString)
	-- Close the file
	f:close()
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

function Waypoint_init()
	for key,value in pairs(waypoint_test) do
		
	end
end		

title = "DM2240 - Week 14 Scripting"
test = false -- Not in use for anything, placeholder 

-- CPlayerInfo start position
CPlayerInfoStartPos = {0,0,10}

-- Wayponts
Waypoint_A_1 = {x=100.0,y=0.0,z=-30.0}
Waypoint_A_2 = {x=0.0,y=0.0,z=-30.0}
Waypoint_A_3 = {x=-100.0,y=0.0,z=-30.0}

waypoint_test = {
					first={x=100.0,y=0.0,z=-30.0},
					second={x=0.0,y=0.0,z=-30.0},
					third={x=-100.0,y=0.0,z=-30.0}
				}

		
--[[ 
COMMENTS
This lua file is read as [KEY] = [VALUE]
There is no need to specify the data type

 theLuaState is like a pointer between the c++ and lua file
 lua_getglobal(theLuaState,varName) 
 >> the pointer between c++ and lua file
 >> varName is to point specifically to which of the following keys/function
 
 lua_pushnumber(theLuaState, value)
 lua_pushstring(theLuaState, value)
 >> the pointer between c++ and lua file
 >> value is the value to put into the pointer, usually used to pass in parameters of a function
 
 lua_call(theLuaState, value, value)
 >> the pointer between c++ and lua FUNCTION
 >> the first value sets the number of parameters
 >> the second value indicates the expected number of returns
 
 lua_tonumber(theLuaState, -1)
>> the pointer betweem the c++ and Lua FUNCTION
>> get the return from the function and put into the value

 lua_pop(theLuaState, 1)
 >> the pointer between c++ and lua file
 >> Remove/Pop the topmost number of the queue
 
 lua_table 
 >> uses {key=value}
-]]
