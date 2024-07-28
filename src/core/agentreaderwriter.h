#ifndef AGENTREADERWRITER_H
#define AGENTREADERWRITER_H

#include <QObject>

#include "brain/fuzzybase.h"
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
     * The method reads a BAF (Brainiac Agent File) format file, which is a plain ASCII based format.
     * It checks if the agent's bones and fuzzies are empty before loading a new agent,
     * otherwise it logs a warning message and aborts the loading process.
     *
     * @return true if the agent is successfully loaded from the file, false otherwise.
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

private:
    Agent *m_agent;
    inline static const QString _indent = "    ";

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
    void addSegment(ConfigBlock &confBlock) const;

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
    void addFuzz(ConfigBlock &confBlock) const;

    void checkAgentEmpty() const;

    /**
     * Parses the fields of the agent and updates the given configuration block.
     *
     * The method takes a list of fields as input and iterates through each field.
     * If the field is equal to "segment", it checks if there are any unknown fields in the configuration block and updates the block's type to SEGMENT.
     * If the field is equal to "endSegment", it processes the segment in the configuration block.
     * If the field is equal to "fuzz", it checks if there are any unknown fields in the configuration block and updates the block's type to FUZZY.
     * If the field is equal to "endFuzz", it processes the fuzzy in the configuration block.
     *
     * @param fields The list of fields to parse.
     * @param confBlock The configuration block to update.
     */
    void parseFields(const QStringList &fields, ConfigBlock &confBlock);
    static void checkUnknown(const ConfigBlock &confBlock);
    void processSegment(ConfigBlock &confBlock) const;
    void processFuzzy(ConfigBlock &confBlock) const;
    static void clearConfigBlock(ConfigBlock &confBlock);

    /**
     * Writes the segment of a bone to the QTextStream.
     *
     * The method writes the segment details of the specified bone to the QTextStream.
     *
     * @param boneId The ID of the bone whose segment is to be written.
     * @param stream The QTextStream object to write the segment to.
     */
    void writeSegment(BrainiacGlobals::BrainiacId boneId, QTextStream& stream) const;

    /**
     * Writes the given fuzz to the provided QTextStream.
     *
     * The method writes the information of the fuzz object to the stream in a specific format.
     * It starts by outputting the fuzz token followed by the fuzz's name.
     * Then, it determines the type of the fuzz and writes the corresponding type token and additional
     * information specific to that type. For example, if the fuzz is an AND type, it writes
     * the mode token and the mode value of the fuzzAnd object.
     * After that, it outputs the editor position of the fuzz. Finally, it outputs the endFuzz token
     * to indicate the end of the fuzz information.
     *
     * @param fuzz A pointer to the FuzzyBase object to be written.
     * @param stream The QTextStream to write the fuzz information to.
     */
    void writeFuzz(FuzzyBase *fuzz, QTextStream& stream) const;
signals:
};

#endif // AGENTREADERWRITER_H
