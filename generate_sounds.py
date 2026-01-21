import wave
import struct
import math
import os

def generate_beep(filename, frequency, duration, sample_rate=22050):
    """Generate a simple beep sound"""
    num_samples = int(sample_rate * duration)
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)  # Mono
        wav_file.setsampwidth(2)  # 16-bit
        wav_file.setframerate(sample_rate)
        
        for i in range(num_samples):
            # Generate square wave for 8-bit effect
            t = i / sample_rate
            value = 0.3 * math.sin(2 * math.pi * frequency * t)
            
            # Add some harmonics for richer sound
            value += 0.15 * math.sin(2 * math.pi * frequency * 2 * t)
            
            # Apply envelope (fade out)
            envelope = 1 - (i / num_samples) ** 0.5
            value *= envelope
            
            # Convert to 16-bit integer
            data = int(value * 32767)
            wav_file.writeframes(struct.pack('<h', data))

def generate_eat_sound():
    """Generate an eating/pickup sound"""
    filename = "Sounds/eat.wav"
    sample_rate = 22050
    duration = 0.15
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        num_samples = int(sample_rate * duration)
        
        for i in range(num_samples):
            t = i / sample_rate
            progress = i / num_samples
            
            # Frequency sweep upward
            freq = 400 + progress * 600
            
            # Square wave
            value = 0.4 if math.sin(2 * math.pi * freq * t) > 0 else -0.4
            
            # Envelope
            envelope = 1 - progress
            value *= envelope
            
            data = int(value * 32767)
            wav_file.writeframes(struct.pack('<h', data))
    
    print(f"Generated {filename}")

def generate_gameover_sound():
    """Generate a game over sound"""
    filename = "Sounds/gameover.wav"
    sample_rate = 22050
    duration = 0.8
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        num_samples = int(sample_rate * duration)
        
        for i in range(num_samples):
            t = i / sample_rate
            progress = i / num_samples
            
            # Frequency sweep downward
            freq = 300 - progress * 200
            
            # Triangle wave
            phase = (freq * t) % 1.0
            value = 0.4 * (abs(phase - 0.5) * 4 - 1)
            
            # Envelope
            envelope = 1 - progress ** 0.3
            value *= envelope
            
            data = int(value * 32767)
            wav_file.writeframes(struct.pack('<h', data))
    
    print(f"Generated {filename}")

def generate_click_sound():
    """Generate a click/menu sound"""
    filename = "Sounds/click.wav"
    sample_rate = 22050
    duration = 0.08
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        num_samples = int(sample_rate * duration)
        
        for i in range(num_samples):
            t = i / sample_rate
            progress = i / num_samples
            
            # High frequency for click
            freq = 800
            
            # Square wave
            value = 0.3 if math.sin(2 * math.pi * freq * t) > 0 else -0.3
            
            # Quick decay
            envelope = 1 - progress ** 0.5
            value *= envelope
            
            data = int(value * 32767)
            wav_file.writeframes(struct.pack('<h', data))
    
    print(f"Generated {filename}")

def generate_background_music():
    """Generate simple background music"""
    filename = "Sounds/music.wav"
    sample_rate = 22050
    duration = 10  # 10 seconds loop
    
    # Simple melody notes (frequencies)
    melody = [523, 587, 659, 587, 523, 440, 494, 523]  # C, D, E, D, C, A, B, C
    note_duration = duration / len(melody)
    
    with wave.open(filename, 'w') as wav_file:
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        for note_idx, freq in enumerate(melody):
            num_samples = int(sample_rate * note_duration)
            
            for i in range(num_samples):
                t = i / sample_rate
                progress = i / num_samples
                
                # Main tone (square wave)
                main = 0.2 if math.sin(2 * math.pi * freq * t) > 0 else -0.2
                
                # Add bass note (octave lower)
                bass = 0.1 if math.sin(2 * math.pi * freq / 2 * t) > 0 else -0.1
                
                value = main + bass
                
                # Envelope for each note
                if progress < 0.1:
                    envelope = progress / 0.1
                elif progress > 0.9:
                    envelope = (1 - progress) / 0.1
                else:
                    envelope = 1.0
                
                value *= envelope
                
                data = int(value * 32767)
                wav_file.writeframes(struct.pack('<h', data))
    
    print(f"Generated {filename}")

if __name__ == "__main__":
    # Create Sounds directory if it doesn't exist
    os.makedirs("Sounds", exist_ok=True)
    
    print("Generating 8-bit sound effects...")
    generate_eat_sound()
    generate_gameover_sound()
    generate_click_sound()
    generate_background_music()
    print("All sounds generated successfully!")
