import { ChildProcess, exec } from "node:child_process";
import iLogContainer from "./iLogContainer";
import iStreamContainer from "./StreamContainer/iStreamContainer";
import StreamContainer from "./StreamContainer/StreamContainer";

import { mkdirSync } from "node:fs";
import { join } from "node:path";


/**
 * Handles the full logic behind having one log container
 * that handles exactly one process
 */

class LogContainer implements iLogContainer{

	private _process: ChildProcess
	private _streams: iStreamContainer

	constructor(command: string, dir: string, name: string){
		this._process = exec(command)
		this._streams = new StreamContainer(this._process, dir, name)


		try {
			mkdirSync(join(dir, name))
		} catch (error) {
			
		}
	}


	get process(){
		return this._process
	}

	get streams(){
		return this._streams
	}

	close(): void {
		process.emit("SIGABRT")
		this.streams.close()
	}
}


export default LogContainer