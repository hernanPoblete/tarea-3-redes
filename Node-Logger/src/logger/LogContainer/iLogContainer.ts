import {ChildProcess} from "node:child_process"
import iStreamContainer from "./StreamContainer/iStreamContainer"

/**
 * Represents a container made for a logger. 
 * It should consist of a process and a series of streams contained in an iStreamContainer 
 */

interface iLogContainer{
	get process(): ChildProcess
	get streams(): iStreamContainer

	/**
	 * Closes the current process using a given code
	 * 
	 */

	close(): void
}


export default iLogContainer