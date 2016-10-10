
void createFile(string name, string content, string path) {
	ofstream outfile ("paretos/VNS/" + path + "/" + name);
	outfile << content << endl;
	outfile.close();
}