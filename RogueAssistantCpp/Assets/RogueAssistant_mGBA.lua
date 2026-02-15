require "RogueAssistant"

console:log("Launching Rogue Assistant...")
rogue_attach()

callbacks:add("frame", rogue_frame)
callbacks:add("shutdown", rogue_shutdown)
callbacks:add("stop", rogue_shutdown)