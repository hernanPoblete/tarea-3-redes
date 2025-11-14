import { createWriteStream, WriteStream } from "fs";
import iStreamContainer from "./iStreamContainer";
import {ChildProcess} from "child_process"
import { join } from "path"




/**
 * Represents the controller for the multiple streams inside a Logger
 */

class StreamContainer implements iStreamContainer{

	private _info: WriteStream
	private _error: WriteStream
	private _full: WriteStream


	constructor(proc: ChildProcess, dir: string, name: string = "Unknown Logger Client-"+(new Date()).getTime()){
		this._error = createWriteStream(join(dir, name, "error.log"), {
			flags: "ax+"
		})

		this._info = createWriteStream(join(dir, name, "info.log"), {
			flags: "ax+"
		})

		this._full = createWriteStream(join(dir, name, "full_info.log"), {
			flags: "ax+"
		})


		proc.stdout.on("data", (msg)=>{
			let msgProcessed = `[${name}::info] ` + msg

			process.stdout.write(msgProcessed);
			this._info.write(msgProcessed);
			this._full.write(msgProcessed);
		})

		proc.stderr.on("data", (msg)=>{
			let msgProcessed = `[${name}::info] ` + msg

			process.stderr.write(msgProcessed);
			this._info.write(msgProcessed);
			this._full.write(msgProcessed);
		})

	}


	get fullStream(){
		return this._full
	}

	get infoStream(){
		return this._info
	}

	get errorStream(){
		return this._error
	}


	close(): void {
		this._info.close()
		this.errorStream.close()
		this.fullStream.close()
	}
}





export default StreamContainer