import * as fs from "fs";
import { env } from "process";
import { dirname } from "path";
import { fileURLToPath } from "url";

export const __filename = fileURLToPath(import.meta.url);
export const __dirname = dirname(__filename);

export const currentDirectory = () => {
  return dirname(fileURLToPath(import.meta.url));
};

export const installEnvironmentConfigs = () => {
  console.log("Installing project environment file");
  fs.writeFile(
    "Data/Configs/Environment.json",
    JSON.stringify(process.env),
    (err) => {
      if (err) {
        throw err;
      }
    }
  );
};

export const installPrebuildConfigs = () => {
  console.log("Installing project pre-build configuration file");
  let configData = {};
};

export const installEngineConfigs = () => {
  console.log("Installing project pre-build configuration file");
  let configData;
};

export const installVulkanConfigs = () => {
  console.log("Installing project pre-build configuration file");
  let configData;
};

export const getJsonObjectFromFile = (jsonFilePath) => {
  const jsonString = fs.readFileSync(jsonFilePath);
  const jsonObject = JSON.parse(jsonString);
  return jsonObject;
};

// import { exec } from "child_process";
// import { setFlagsFromString } from "v8";

// export const parseTerminalCommand = (processArgv) => {
//   return processArgv.slice(2);
// };

// export const __filename = fileURLToPath(import.meta.url);
// export const __dirname = dirname(__filename);

// export const executeOSCommand = (command) => {
//   exec(command, (err, stdout, stderr) => {
//     if (err) {
//       console.error(`Failed to execute the command: ${err.message}`);
//       return;
//     }
//     if (stderr) {
//       console.error(`Failed to execute the command: ${stderr}`);
//       return;
//     }
//     console.log(`${stdout}`);
//   });
// };

// export const getJsonObjectFromFile = (jsonFilePath) => {
//   const jsonString = fs.readFileSync(jsonFilePath);
//   const jsonObject = JSON.parse(jsonString);
//   return jsonObject;
// };
