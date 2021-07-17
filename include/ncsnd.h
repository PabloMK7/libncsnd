/**
 * @file ncsnd.h
 * @brief CSND interface for the Nintendo 3DS
 */
#pragma once

#include <3ds/types.h>

extern Handle ncsndCSNDHandle;

#define NCSND_DIRECTSOUND_MAX_VOLUME 32768

/// Output modes for a direct sound.
typedef enum
{
	NCSND_SOUNDOUTPUT_MONO = 0,
	NCSND_SOUNDOUTPUT_STEREO = 1
} ncsndSoundOutputMode;

typedef enum
{
	NCSND_ENCODING_PCM8 = 0, ///< PCM8
	NCSND_ENCODING_PCM16,    ///< PCM16
	NCSND_ENCODING_ADPCM,    ///< IMA-ADPCM
} ncsndSoundFormat;

typedef struct
{
    u16 data;
    u8 tableIndex;
    u8 padding;
} ncsndDirectSoundIMAADPCMContext;

/// Channel related data for a direct sound.
typedef struct 
{
	u8 channelAmount; ///< Amount of channels used, either 1 or 2.
	u8 channelEncoding; ///< Sample encoding.
	bool isLeftPhys; ///< If leftSampleData is a physical address (ignore if unknown).
	bool isRightPhys; ///< If rightSampleData is a physical address (ignore if unknown).
	u32 sampleRate; ///< Sample rate for both channels.
	void* leftSampleData; ///< Pointer to the left ear or mono sample data.
	void* rightSampleData; ///< Pointer to the right ear or mono sample data.
	u32 sampleDataLength; ///< Size of an individual sample data buffer in bytes.
	ncsndDirectSoundIMAADPCMContext leftAdpcmContext; ///< IMA ADPCM context for left ear or mono channel.
	ncsndDirectSoundIMAADPCMContext rightAdpcmContext; ///< IMA ADPCM context for right ear channel.
} ncsndDirectSoundChannelData;

/// Modifiers applied to a direct sound.
typedef struct
{
	float speedMultiplier; ///< Sound playback speed, default: 1.
	s32 channelVolumes[2]; ///< Volume of each individual channel, max: 32768.
	u8 unknown0; ///< Unknown
	u8 padding[3]; ///< Padding? Never used.
	float unknown1; ///< Unknown, seems to be set to 1. Some sort of play delay?
	u32 unknown2; ///< Unknown, maybe related to unknown1. Some sort of play delay?
	u8 ignoreVolumeSlider; ///< Set to 1 to play at maximum volume, ignoring the volume slider.
	u8 forceSpeakerOutput; ///< Set to 1 to play on the speakers, even if headphones are connected.
	u8 playOnSleep; ///< Set to 0 to pause the sound on sleep and 1 to continue playing on sleep. 
	u8 padding1; ///< Padding? Never used.
} ncsndDirectSoundModifiers;

/// Direct sound struct.
typedef struct
{
	u8 always0; ///< Always set to 0 by applets.
	u8 soundOutputMode; ///< Output mode (ncsndSoundOutputMode).
	u8 padding[2]; ///< Padding? Never used.
	ncsndDirectSoundChannelData channelData; ///< Channel related data.
	ncsndDirectSoundModifiers soundModifiers; ///< Modifiers applied to sound playback.
} ncsndDirectSound;

typedef struct
{
	bool isPhysAddr;
	void* sampleData;
	void* loopSampleData;
	u32 totalSizeBytes;

	ncsndSoundFormat encoding;
	bool loopPlayback;
	ncsndDirectSoundIMAADPCMContext context;
	ncsndDirectSoundIMAADPCMContext loopContext;

	u32 sampleRate;
	float volume;
	float pitch;
	float pan;
	bool linearInterpolation;

} ncsndSound;

extern u32 ncsndChannels;      ///< Bitmask of channels that are allowed for usage.

/// Initializes CSND.
Result ncsndInit(bool installAptHook);

/// Exits CSND.
void ncsndExit(void);

/**
 * @brief Initializes a direct sound to the default values.
 * @param sound Pointer to a direct sound struct.
*/
void ncsndInitializeDirectSound(ncsndDirectSound* sound);

/**
 * @brief Plays a direct sound.
 * @param sound Pointer to a direct sound struct.
 * @param chn Direct sound channel to use. Range [0, 3].
 * @param priority Direct sound priority, used if the channel is already playing. Smaller value -> Higher priority. Range [0, 31].
 * @param isVAddr Whether sound.channelData.sampleData hold virtual addresses (true) or physical addresses (false).
*/
Result ncsndPlayDirectSound(u32 chn, u32 priority, ncsndDirectSound* sound);

/**
 * @brief Plays a sound.
 * @param chn Channel to play the sound on.
 * @param flags Flags containing information about the sound.
 * @param sampleRate Sample rate of the sound.
 * @param vol The volume, ranges from 0.0 to 1.0 included.
 * @param pan The pan, ranges from -1.0 to 1.0 included.
 * @param data0 First block of sound data.
 * @param data1 Second block of sound data. This is the block that will be looped over.
 * @param size Size of the sound data.
 *
 * In this implementation if the loop mode is used, data1 must be in the range [data0 ; data0 + size]. Sound will be played once from data0 to data0 + size and then loop between data1 and data0+size.
 */
Result ncsndPlaySound(u32 chn, ncsndSound* sound);

void ncsndPauseSound(u32 chn, bool pause);

void ncsndStopSound(u32 chn);

/**
 * @brief Gets whether a channel is playing.
 * @param channel Channel to check.
 * @param status Pointer to output the channel status to.
 */
bool ncsndIsPlaying(u32 chn);
