import Logger from "./logger/Logger"
import { join } from "node:path";

let cwd = process.cwd();

const Router_Logger = new Logger();
const bin_name:string = process.argv[2]||"Router.out";

for(let i = 1; i<=5; i++){
	let routing_table = join(cwd, `Routes/rutas_R${i}_v3_mtu.txt`);
	let tot_name = join(cwd, bin_name);

	Router_Logger.startProcess(`Router ${i}`, `${tot_name} 127.0.0.1 888${i} ${routing_table}`)
}