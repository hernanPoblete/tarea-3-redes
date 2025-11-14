import { ChildProcess } from "node:child_process"
import HashMap from "hashmap"


/**
 * Represents all the tools a logger used by this application will use
 * to work
 */
interface iLogger{

	/**
	 * Starts a process using the logger
	 * @param name the name for this process to be identified
	 * @param command the command to start the process
	 */
	startProcess(name: string, command: string): void | Promise <void>

	/**
	 * Closes a process being used by the logger
	 * @param name The name identifying the process inside the logger
	 * @param code The code to use when closing the process
	 */
	close(name: string, code: number):void

	/**
	 * Given the name looked for, returns the process associated with said name
	 * @param name The name of the process running
	 * @returns A ChildProcess containing the referenced process or undefined if name does not exist
	 */

	getProcess(name: string): ChildProcess|undefined
}


export default iLogger