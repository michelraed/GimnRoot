

void ProcessCoincidenceFile(TString fileName,
                            TString processedFolder = "",
                            TString identificatorPath = "",
                            TString calibrationFile = "",
                            TString alignmentFile = "",
                            TString method = "",
                            TString plotsFolder = "")
{

    const UInt_t nProcesses = 2; // Number of processes in paralells

    // Instantiates our Multi threading class
    MultiThreaderGimn mt(nProcesses);

    DIFERENTIATOR = 0;
    TString contador = TString(std::to_string(DIFERENTIATOR));

    std::vector<TString> aqdData{fileName,
                                 identificatorPath,
                                 calibrationFile,
                                 alignmentFile,
                                 processedFolder,
                                 plotsFolder,
                                 method,
                                 contador};

    auto runGimnCoinc = [](std::vector<TString> aqdData)
    {
        ProcessAcquisition aqd(std::stoi(aqdData[7].Data()));

        if (aqdData[1].IsNull())
        {
            std::cout << " Please set the Identificators Path" << std::endl;
            return;
        }
        else
            aqd.setIdentificator(aqdData[1].Data());

        if (aqdData[2].IsNull())
        {
            std::cout << " No calibration was given" << std::endl;
        }
        else
            aqd.setCalibrationFactors(aqdData[2].Data());

        aqd.initializeEmpty();
        if (!(aqdData[3].IsNull()))
        {
            aqd.setAligmentFileName(aqdData[3].Data());
        }

        if (aqdData[4].IsNull())
        {
            std::vector<std::string> foldName = splitString(aqdData[0].Data(), '/');
            foldName.pop_back();
            aqdData[4] = TString(joinString(foldName, '/'));
            aqd.setProcessedFolder(aqdData[4].Data());
        }
        else
            aqd.setProcessedFolder(aqdData[4].Data());

        if (!(aqdData[5].IsNull()))
        {
            aqd.setPlotsFolder(aqdData[5].Data());
        }

        if (aqdData[6].IsNull())
        {
            aqd.setMethod("PetSys");
        }
        else
            aqd.setMethod(aqdData[6].Data());

        aqd.processCoincidenceTomographic(aqdData[0].Data());
        aqd.clear();
    };

    mt.createThread(runGimnCoinc, aqdData);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    mt.waitForCompletion();
}

void runAlignment(std::string alignmentAQD,
                  std::string identificatorPath,
                  std::string calibrationFolder,
                  std::string calibrationFactors,
                  std::string plotsFolder,
                  std::string processedFolder

)
{
    ProcessAcquisition aqd(0);
    aqd.setIdentificator(identificatorPath);
    aqd.initializeEmpty();
    aqd.setCalibrationFactors(calibrationFactors);
    aqd.setProcessedFolder(processedFolder);
    aqd.setMethod("PetSys");
    aqd.setVerbose(true);
    aqd.setAlignmentFolder(calibrationFolder);
    aqd.setObtainAlignmentData(true);
    aqd.setPlotsFolder(plotsFolder);
    aqd.processCoincidenceTomographic(alignmentAQD);
    aqd.clear();
}

void processGroupFile(TString fileName,
                      TString identificatorPath,
                      TString calibrationFile,
                      TString alignmentFile,
                      TString processedFolder,
                      TString method,
                      TString plotsFolder)
{

    ProcessAcquisition aqd(fileName.Data(), identificatorPath.Data());

    aqd.setCalibrationFactors(calibrationFile.Data());
    aqd.setAligmentFileName(alignmentFile.Data());
    aqd.setProcessedFolder(processedFolder.Data());
    if (!plotsFolder.IsNull())
        aqd.setPlotsFolder(plotsFolder.Data());

    aqd.setMethod("PetSys");
    aqd.ProcessGroupFromRaw();
    aqd.clear();
}

void runCalibration(std::string calibrationAQD,
                    std::string identificatorPath,
                    std::string calibrationFolder,
                    std::string plotsFolder,
                    std::string processedFolder

)
{
    ProcessAcquisition aqd(calibrationAQD, identificatorPath);
    // ProcessAcquisition aqd(calibrationAQD);
    // aqd.setIdentificator(identificatorPath);
    // aqd.initializeEmpty();
    aqd.setCalibrationFolder(calibrationFolder);
    aqd.setPlotsFolder(plotsFolder);
    aqd.setProcessedFolder(processedFolder);
    aqd.setMethod("PetSys");
    aqd.ProcessGroupFromRaw();
    aqd.writeCalibration();
    aqd.clear();
    aqd.ProcessGroupFromRaw();
    aqd.clear();
}

void ProcessTomography(TString aimFolder, bool processAll = false)
{

    TString baseFolder = aimFolder;
    TString cleanBaseFolder = baseFolder.Strip(TString::kTrailing, '/');

    if (checkNeededFiles(baseFolder))
    {

        TString identificatorPath = cleanBaseFolder + "/identificators.conf";
        TString calibrationAQDFolder = cleanBaseFolder + "/CalibrationAQD";
        TString backgroundAQDFolder = cleanBaseFolder + "/BackgroundAQD";
        TString tomographyAQDFolder = cleanBaseFolder + "/TomographyAQD";

        TString calibrationRaw = findCalibrationFile(calibrationAQDFolder, "group");
        TString alignmentRaw = findCalibrationFile(calibrationAQDFolder, "coinc");

        if (calibrationRaw.IsNull())
        {
            std::cout << " Faltando arquivo group para a calibração dos espectros de energia" << std::endl;
        }
        if (alignmentRaw.IsNull())
        {
            std::cout << " Faltando arquivo group para a Alinhamento dos sensores " << std::endl;
        }

        // Se as pastas nao existirem, serão criadas;
        checkProcessingPaths(baseFolder);

        TString processedPath = cleanBaseFolder + "/OutputData/Processed";
        TString calibrationParametersPath = cleanBaseFolder + "/OutputData/CalibrationParameters";
        TString resultPath = cleanBaseFolder + "/OutputData/Results";
        TString plotsPath = cleanBaseFolder + "/OutputData/Plots";

        bool align{false}, calibrate{false};
        TString alignmentCorrectionPath = fileWithNameExists(calibrationParametersPath, "AlignmentCorrection");
        if (alignmentCorrectionPath.IsNull())
        {
            std::cout << "Nao Existe arquivo de Alinhamento deseja criar?" << std::endl;
            if (askUserToContinue())
            {
                if (alignmentRaw.IsNull())
                {
                    std::cout << "Arquivo de coincidencias nescessário para gerar arquivo de alinhamento faltando..." << std::endl;
                    std::cout << "Por favor checar a existencia do arquivo na pasta a pasta : " << std::endl;
                    std::cout << calibrationAQDFolder << std::endl;
                    return;
                }

                else
                {
                    align = true;
                }
            }
        }

        TString calibrationFactorsPath = fileWithNameExists(calibrationParametersPath, "CalibrationFactors");
        if (calibrationFactorsPath.IsNull())
        {
            std::cout << "Nao Existe arquivo de Calibração" << std::endl;
            if (askUserToContinue())
            {

                if (calibrationRaw.IsNull())
                {
                    std::cout << "Arquivo de grupos nescessário para gerar arquivo de calibração faltando..." << std::endl;
                    std::cout << "Por favor checar a existencia do arquivo na pasta a pasta : " << std::endl;
                    std::cout << calibrationAQDFolder << std::endl;
                    return;
                }
                else
                {

                    calibrate = true;
                }
            }
        }
        if (calibrate)
        {

            runCalibration(calibrationRaw.Data(),
                           identificatorPath.Data(),
                           calibrationParametersPath.Data(),
                           plotsPath.Data(),
                           processedPath.Data());
        }

        if (align)
        {
            calibrationFactorsPath = fileWithNameExists(calibrationParametersPath, "CalibrationFactors");
            if (calibrationFactorsPath.IsNull())
                std::cout << "impossivel de gerar dados de alinhamento, faltando arquivo de calibração para as energias. " << std::endl;
            else
                runAlignment(alignmentRaw.Data(),
                             identificatorPath.Data(),
                             calibrationParametersPath.Data(),
                             calibrationFactorsPath.Data(),
                             plotsPath.Data(),
                             processedPath.Data());

            alignmentCorrectionPath = fileWithNameExists(calibrationParametersPath, "AlignmentCorrection");
        }

        auto runGimnCoinc = [](std::vector<TString> aqdData)
        {
            ProcessAcquisition aqd(std::stoi(aqdData[6].Data()));
            aqd.setIdentificator(aqdData[0].Data());
            aqd.setCalibrationFactors(aqdData[1].Data());
            aqd.setProcessedFolder(aqdData[4].Data());
            aqd.setMethod("PetSys");
            aqd.initializeEmpty();
            // aqd.setVerbose(true);
            aqd.setAligmentFileName(aqdData[2].Data());
            // aqd.setPlotsFolder(aqdData[3].Data());
            aqd.processCoincidenceTomographic(aqdData[5].Data());
            aqd.clear();
        };

        GenerateBackgroundCopiesFromTemplate(backgroundAQDFolder, tomographyAQDFolder, 15);

        // Number of processes in paralells
        const UInt_t nProcesses = 32;
        // Instantiates our Multi threading class
        MultiThreaderGimn mt(nProcesses);

        if (processAll)
        {
            DIFERENTIATOR = 0;
            std::vector<TString> tomographicAcquisitions = checkTomographyAQDS(cleanBaseFolder);

            for (auto &aqd : tomographicAcquisitions)
            {
                TString tomographicFolder = aqd.Strip(TString::kTrailing, '/');

                std::vector<std::string> tomographicFiles = checkIfTomographyc(aqd.Data());

                std::string val = aqd.Data();

                auto aux = splitString(val, '/');

                TString outPath = processedPath + "/" + TString(aux[aux.size() - 1]);

                ensureFolderExists(outPath);
                for (auto &file : tomographicFiles)
                {
                    if (file.find("coin") != -1)
                    {
                        std::cout << "Coincidence file Found..." << std::endl;
                        std::cout << "Processing File :" << file << std::endl;

                        TString contador = std::to_string(DIFERENTIATOR);

                        TString fileName = tomographicFolder + "/" + TString(file);
                        std::vector<TString> aqdData{
                            identificatorPath,
                            calibrationFactorsPath,
                            alignmentCorrectionPath,
                            plotsPath,
                            outPath,
                            fileName,
                            contador};

                        mt.createThread(runGimnCoinc, aqdData);
                        DIFERENTIATOR++; // Proximo core
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
                mt.waitForCompletion();
            }

            std::vector<TString> processedAQD = ListSubdirectories(processedPath);

            for (auto &aqd : processedAQD)
            {
                MergeAndReplaceOutput(aqd.Data(), resultPath.Data());
            }
        }
        else
        {
            std::cout << " Selecione a tomografia desejada:" << std::endl;
            
            TString aqd = SelectFolderFromDirectory(tomographyAQDFolder);





            TString tomographicFolder = aqd.Strip(TString::kTrailing, '/');

            std::vector<std::string> tomographicFiles = checkIfTomographyc(aqd.Data());

            std::string val = aqd.Data();

            auto aux = splitString(val, '/');

            TString outPath = processedPath + "/" + TString(aux[aux.size() - 1]);

            ensureFolderExists(outPath);
            for (auto &file : tomographicFiles)
            {
                if (file.find("coin") != -1)
                {
                    std::cout << "Coincidence file Found..." << std::endl;
                    std::cout << "Processing File :" << file << std::endl;

                    TString contador = std::to_string(DIFERENTIATOR);

                    TString fileName = tomographicFolder + "/" + TString(file);
                    std::vector<TString> aqdData{
                        identificatorPath,
                        calibrationFactorsPath,
                        alignmentCorrectionPath,
                        plotsPath,
                        outPath,
                        fileName,
                        contador};

                    mt.createThread(runGimnCoinc, aqdData);
                    DIFERENTIATOR++; // Proximo core
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                }
            }
            mt.waitForCompletion();
        }

        TString OutOrganization = cleanBaseFolder + "/data_organization.md";
        GenerateFolderStructureMarkdown(cleanBaseFolder, OutOrganization);
    }
}

void TomographyProcessingMenu()
{
    TString identificatorPath;
    TString calibrationAQDFolder;
    TString backgroundAQDFolder;
    TString tomographyAQDFolder;
    TString calibrationRaw;
    TString alignmentRaw;
    TString processedPath;
    TString calibrationParametersPath;
    TString resultPath;
    TString plotsPath;
TString individualFiles;
    std::string input;
    // Seleciona a pasta da tomografia
    TString folderPath;
    std::cout << "☢️ Bem-vindo ao programa de processamento de tomografias!\n📁 Primeiramente digite o caminho da pasta de tomografia.\n❗Caso deseje simplesmente prosseguir digite 'continuar' ";
    do
    {
        std::getline(std::cin, input);
        if (input == ".q")
            return;
        if (input == "continuar")
            break;
        folderPath = input.c_str();
    } while (gSystem->AccessPathName(folderPath, kFileExists));

    if (folderPath.IsNull())
    {
        std::cout << "❗📁 Nenhuma pasta contendo as aquisições tomográficas foi informada." << std::endl;

        while (true)
        {
            std::cout << "\n\n";
            std::cout << "☢️  Bem-vindo ao Menu de Processamento do Tomógrafo!\n";
            std::cout << "Utilize as opções abaixo para reconstruir, processar ou analisar arquivos de tomografia.\n";
            std::cout << "❗ Para sair, digite '.q' a qualquer momento.\n\n";

            std::cout << "📋 Opções do Menu Principal:\n";
            std::cout << " [1] 📁 Reconstruir Tomografia\n";
            std::cout << " [2] 📄 Processar Arquivo Individual Dentro da Estrutura de Pastas\n";

            std::cout << "Digite sua opção: ";

            std::getline(std::cin, input);
            if (input == ".q")
                return;

            else if (input == "1")
            {
                TString identificators, calibration, alignment, outFolder;
                std::string option;
                TString selectedPath;
                std::cout << "📁 Digite a pasta contendo a tomografia a ser reconstruida: ";
                std::cout << "❗ Para sair, digite '.q' a qualquer momento.\n\n";
                do
                {
                    std::getline(std::cin, option);
                    if (option == ".q")
                        return;
                    selectedPath = TString(option);
                } while (gSystem->AccessPathName(selectedPath.Data(), kFileExists));

                // Arquivo de identificadores
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de identificadores: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        identificators = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Arquivo de calibração
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de calibração: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        calibration = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Arquivo de alinhamento
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de alinhamento: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        alignment = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Pasta de saída
                while (true)
                {
                    std::cout << "📁 Digite a pasta de saída: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    // Verifica se existe e é diretório
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        outFolder = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Pasta inválida. Tente novamente ou digite .q para sair.\n";
                    }
                }

                auto runGimnCoinc = [](std::vector<TString> aqdData)
                {
                    ProcessAcquisition aqd(std::stoi(aqdData[6].Data()));
                    aqd.setIdentificator(aqdData[0].Data());
                    aqd.setCalibrationFactors(aqdData[1].Data());
                    aqd.setProcessedFolder(aqdData[4].Data());
                    aqd.setMethod("PetSys");
                    aqd.initializeEmpty();
                    // aqd.setVerbose(true);
                    aqd.setAligmentFileName(aqdData[2].Data());
                    // aqd.setPlotsFolder(aqdData[3].Data());
                    aqd.processCoincidenceTomographic(aqdData[5].Data());
                    aqd.clear();
                };

                // Number of processes in paralells
                const UInt_t nProcesses = 32;
                // Instantiates our Multi threading class
                MultiThreaderGimn mt(nProcesses);

                TString tomographicFolder = selectedPath.Strip(TString::kTrailing, '/');

                std::vector<std::string> tomographicFiles = checkIfTomographyc(tomographicFolder.Data());

                std::string val = tomographicFolder.Data();

                auto aux = splitString(val, '/');

                TString outPath = processedPath + "/" + TString(aux[aux.size() - 1]);

                ensureFolderExists(outPath);
                for (auto &file : tomographicFiles)
                {
                    if (file.find("coin") != -1)
                    {
                        std::cout << "Coincidence file Found..." << std::endl;
                        std::cout << "Processing File :" << file << std::endl;

                        TString contador = std::to_string(DIFERENTIATOR);


                        TString fileName = tomographicFolder + "/" + TString(file);
                        std::vector<TString> aqdData{
                            identificators,
                            calibration,
                            alignment,
                            "",
                            outFolder,
                            fileName,
                            contador};

                        mt.createThread(runGimnCoinc, aqdData);
                        DIFERENTIATOR++; // Proximo core
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
                mt.waitForCompletion();
            }

            else if (input == "2")
            {
                TString file, identificators, calibration, alignment, outFolder;

                // Arquivo group ou coincidence
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo group ou coincidence: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        file = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Arquivo de identificadores
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de identificadores: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        identificators = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Arquivo de calibração
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de calibração: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        calibration = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Arquivo de alinhamento
                while (true)
                {
                    std::cout << "📄 Digite o caminho do arquivo de alinhamento: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        alignment = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Arquivo inválido. Tente novamente ou digite .q para sair.\n";
                    }
                }

                // Pasta de saída
                while (true)
                {
                    std::cout << "📁 Digite a pasta de saída: ";
                    std::getline(std::cin, input);
                    if (input == ".q")
                        return;
                    // Verifica se existe e é diretório
                    if (!gSystem->AccessPathName(input.c_str()))
                    {
                        outFolder = input.c_str();
                        break;
                    }
                    else
                    {
                        std::cout << "❌ Pasta inválida. Tente novamente ou digite .q para sair.\n";
                    }
                }

                if (file.Contains("group"))
                    processGroupFile(file, identificators, calibration, alignment, outFolder, "PetSys", "");
                else if (file.Contains("coin"))
                    ProcessCoincidenceFile(file, outFolder, identificators, calibration, alignment, "PetSys");
                else
                    std::cout << "❗ Arquivo não reconhecido como group ou coin.\n";
            }
        }
    }

    else
    {
        if (checkNeededFiles(folderPath))
        {
            TString baseFolder = folderPath;
            TString cleanBaseFolder = baseFolder.Strip(TString::kTrailing, '/');
            identificatorPath = cleanBaseFolder + "/identificators.conf";
            calibrationAQDFolder = cleanBaseFolder + "/CalibrationAQD";
            backgroundAQDFolder = cleanBaseFolder + "/BackgroundAQD";
            tomographyAQDFolder = cleanBaseFolder + "/TomographyAQD";
            calibrationRaw = findCalibrationFile(calibrationAQDFolder, "group");
            alignmentRaw = findCalibrationFile(calibrationAQDFolder, "coinc");
            checkProcessingPaths(baseFolder);
            processedPath = cleanBaseFolder + "/OutputData/Processed";
            calibrationParametersPath = cleanBaseFolder + "/OutputData/CalibrationParameters";
            resultPath = cleanBaseFolder + "/OutputData/Results";
            plotsPath = cleanBaseFolder + "/OutputData/Plots";
            individualFiles = cleanBaseFolder + "/OutputData/IndividualFiles";
            TString calibrationFactorsPath = fileWithNameExists(calibrationParametersPath, "CalibrationFactors");
            TString alignmentCorrectionPath = fileWithNameExists(calibrationParametersPath, "AlignmentCorrection");

            std::string inputMain;
            while (true)
            {
                std::cout << "\n\n";
                std::cout << "☢️  Bem-vindo ao Menu de Processamento do Tomógrafo!\n";
                std::cout << "Utilize as opções abaixo para reconstruir, processar ou analisar arquivos de tomografia.\n";
                std::cout << "❗ Para sair, digite '.q' a qualquer momento.\n\n";

                std::cout << "📋 Opções do Menu Principal:\n";
                std::cout << " [1] 📁 Reconstruir Tomografia (Processar Todos os Arquivos da Pasta)\n";
                std::cout << " [2] 📁 Reconstruir Tomografia específica\n";
                std::cout << " [3] 📄 Processar Arquivo Group ou Coincidence Dentro da Estrutura de Pastas\n";
                std::cout << " [4] 📄 Processar Arquivo Group ou Coincidence fora deste projeto, porém com os mesmos arquivos de calibração\n";

                std::cout << "Digite sua opção: ";

                std::getline(std::cin, inputMain);
                if (inputMain == ".q")
                    return;

                if (inputMain == "1")
                {
                    ProcessTomography(baseFolder, true);
                }
                else if (inputMain == "2")
                {
                    ProcessTomography(baseFolder, false);
                }
                else if (inputMain == "3")
                {
                    TString selected = SelectFileFromFolder(individualFiles);
                    while (selected.IsNull())
                    {
                        selected = SelectFileFromFolder(individualFiles);
                        if (selected.Contains(".q"))
                            return;
                    }
                    if (selected.Contains("group"))
                        processGroupFile(selected, identificatorPath, calibrationFactorsPath, alignmentCorrectionPath, processedPath, "PetSys", plotsPath);
                    else if (selected.Contains("coin"))
                        ProcessCoincidenceFile(selected, processedPath, identificatorPath, calibrationFactorsPath, alignmentCorrectionPath, "PetSys",plotsPath);
                }
                else if (inputMain == "4")
                {
                    std::string option4in;
                    TString selectedPath;
                    std::cout << "📁 Digite a pasta contendo o arquivo group ou coincidence: ";
                    std::cout << "❗ Para sair, digite '.q' a qualquer momento.\n\n";
                    do
                    {
                        std::getline(std::cin, option4in);
                        if (option4in == ".q")
                            return;
                        selectedPath = TString(option4in);
                    } while (gSystem->AccessPathName(selectedPath.Data(), kFileExists));

                    TString selected = SelectFileFromFolder(selectedPath);
                    while (selected.IsNull())
                    {
                        selected = SelectFileFromFolder(selectedPath);
                        if (selected.Contains(".q"))
                            return;
                    }
                    if (selected.Contains("group"))
                        processGroupFile(selected, identificatorPath, calibrationFactorsPath, alignmentCorrectionPath, processedPath, "PetSys", plotsPath);
                    else if (selected.Contains("coin"))
                        ProcessCoincidenceFile(selected, processedPath, identificatorPath, calibrationFactorsPath, alignmentCorrectionPath, "PetSys",plotsPath);
                }
            }
        }
    }
}
