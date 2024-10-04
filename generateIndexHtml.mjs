#!/usr/bin/env zx

import { readFile, writeFile, rm } from "fs/promises";
const inputZones = JSON.parse(await stdin());

const indexTemplateFile = await readFile("./templates/index.html", "utf8");

const timezonesOptions = inputZones
	.map(z => `<option value="${z.offset}">${z.timezone}</option>`)
	.join("\n");

// Delete if exists
await rm("./data/index.html", { force: true });
// Replace the template with the actual data
await writeFile(
	"./data/index.html",
	indexTemplateFile.replace("<!--PLACEHOLDER-->", timezonesOptions)
);
