#ifndef AGENTREADERWRITER_H
#define AGENTREADERWRITER_H

#include <QObject>
#include "src/core/agent.h"

class AgentReaderWriter : public QObject
{
    Q_OBJECT
public:
    enum ConfigBlockType { UNKNOWN, FUZZY, SEGMENT };

    struct ConfigBlock {
        ConfigBlockType type = AgentReaderWriter::UNKNOWN;
        BrainiacGlobals::ItemType itemType = BrainiacGlobals::UNKNOWN;
        QStringList lines;
    };

    explicit AgentReaderWriter(Agent *parent);

    /**
     * Loads the agent from a BAF file.
     *
     * This method reads agent data from a Brainiac Agent File (BAF) format. The BAF format is a plain ASCII based format
     * that is human readable and easily editable. It can be understood by external tools or scripts as well.
     *
     * Before loading, this method checks if the agent has any bones or fuzzies. A new agent can only be loaded if it is empty,
     * i.e., if it doesn't have any bones or fuzzies. If the agent has bones or fuzzies, an error message will be logged and
     * loading will be aborted.
     *
     * The method opens the BAF file in read-only mode using QFile and creates a QTextStream to read the contents of the file.
     * It then iterates over each line in the file and processes the fields in each line. If a field with the value "segment"
     * is found, a new ConfigBlock is created with the type set to ConfigBlockType::SEGMENT. If a field with the value "endSegment"
     * is found, the current ConfigBlock is dispatched and a new one is created. The lines of the ConfigBlock are appended to
     * the ConfigBlock and the type is set. Once all lines are processed, the file is closed and the method returns whether the
     * loading was successful or not.
     *
     * @return true if the agent was successfully loaded, false otherwise.
     */
    [[nodiscard]] bool loadFromBAF();

    [[nodiscard]] bool save();

    /**
     * Saves the agent as a BAF file.
     *
     * A BAF file is the "Brainiac Agent File" format. The BAF format is a plain ASCII based format
     * that is human readable and easily editable. It can be understood by external tools or scripts as well.
     *
     * @return true if the agent was successfully saved, false otherwise.
     */
    [[nodiscard]] bool saveAsBAF() const;

    /**
     * Adds a segment to the agent.
     *
     * This method is used to add a segment to the agent based on the provided ConfigBlock.
     * The ConfigBlock contains information about the segment, such as its type and properties.
     *
     * The method iterates over each line in the ConfigBlock and processes the fields in each line.
     * If a field with the value "segment" is found, it indicates the start of a new segment. The name
     * of the segment is stored in the variable 'segmentName'.
     *
     * If a field with the value "parent" is found, it indicates the parent of the current segment.
     * The parent is used to determine the hierarchical relationship between segments.
     *
     * If fields with the values "translation", "rotation", and "size" are found, they represent the
     * position, orientation, and size of the segment, respectively.
     *
     * After processing all the lines in the ConfigBlock, the method creates a new BoneBox with the
     * specified segment properties. The BoneBox is then added to the agent's body.
     *
     * @param confBlock The ConfigBlock containing information about the segment.
     */
    void addSegment(ConfigBlock &confBlock);

    /**
     * Adds a fuzz to the agent.
     *
     * This method adds a fuzz to the agent based on the provided ConfigBlock. The ConfigBlock should contain the necessary
     * information to create the fuzz, including its type and attributes.
     *
     * The method first checks if the fuzz type is found in the ConfigBlock. If it is not found, the method does nothing.
     *
     * If the fuzz type is found, the method creates a FuzzyBase pointer, fuzz, and initializes it to nullptr. The method then
     * checks the itemType of the ConfigBlock to determine which type of fuzz to create.
     *
     * For example, if the itemType is BrainiacGlobals::NOISE, the method calls the addNoiseNode method of the agent's brain, passing in
     * the newId parameter, and assigns the returned fuzz to the fuzz pointer.
     *
     * After creating the fuzz, the method iterates over each line in the ConfigBlock. For each line, the method splits the line
     * into fields using a space as the delimiter.
     *
     * @param confBlock The ConfigBlock containing the information to create the fuzz.
     *
     * @see FuzzyBase, BrainiacGlobals::ItemType, addNoiseNode
     */
    void addFuzz(ConfigBlock &confBlock);

private:
    Agent *m_agent;
    inline static const QString _indent = "    ";

signals:
};

#endif // AGENTREADERWRITER_H
