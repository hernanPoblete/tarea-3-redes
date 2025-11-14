import { WriteStream } from "node:fs";


/**
 * Represents the set of streams contained within a logger
 */

interface iStreamContainer{
	get infoStream(): WriteStream
	get errorStream(): WriteStream
	get fullStream(): WriteStream

	/**
	 * Closes all the streams
	 */
	close(): void
}


export default iStreamContainer