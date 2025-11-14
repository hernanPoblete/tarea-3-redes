import {exec, ChildProcess} from "node:child_process"
import { createWriteStream, WriteStream, mkdirSync } from "node:fs"

import iLogContainer from "./LogContainer/iLogContainer"

import HashMap from "hashmap"
import iLogger from "./iLogger"

import { join } from "node:path"
import LogContainer from "./LogContainer/LogContainer"
import iStreamContainer from "./LogContainer/StreamContainer/iStreamContainer"


class Logger implements iLogger{
	
	private processes: HashMap<string, iLogContainer> = new HashMap()
	private creation_stamp = (new Date()).getTime()
	private dir: string

	private streams: iStreamContainer

	constructor(dir: string = `${join(process.cwd(), "logs")}`, uid=this.creation_stamp.toString()){
		this.dir = join(dir, uid)

		try{
			mkdirSync(this.dir,{
				recursive:true
			})
		}catch{

		}
	}


	async startProcess(name: string, command: string) {

		let p = this.processes.get(name)
		if (p){
			throw new Error(`Given key ${name} is already in use by process with id ${p.process.pid}`)
		}

		let container: iLogContainer = new LogContainer(command, this.dir, name)

		this.processes.set(name, container)
		
	}

	async close(name: string) {
		let proc = this.processes.get(name)
		proc.close()
		this.processes.delete(name)
	}

	getProcess(name: string): ChildProcess|undefined {
		return this.processes.get(name).process
	}
}


export default Logger