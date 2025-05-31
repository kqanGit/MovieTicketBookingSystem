#include "SFMLUIManager.h"
#include <sstream>
#include <iomanip>

// Function to add a pending showtime to the list
void SFMLUIManager::addPendingShowtime() {
    // Validate inputs
    if (newShowtimeDate.empty() || newShowtimeStartTime.empty() || newShowtimeEndTime.empty()) {
        showSuccessMessage("Please fill in all showtime fields (Date, Start Time, End Time).");
        return;
    }
    
    // Check date format (simple validation)
    if (newShowtimeDate.length() != 10 || 
        newShowtimeDate[4] != '-' || 
        newShowtimeDate[7] != '-') {
        showSuccessMessage("Date format should be YYYY-MM-DD");
        return;
    }
    
    // Check time format (simple validation)
    if (newShowtimeStartTime.length() != 5 || newShowtimeStartTime[2] != ':' ||
        newShowtimeEndTime.length() != 5 || newShowtimeEndTime[2] != ':') {
        showSuccessMessage("Time format should be HH:MM");
        return;
    }
    
    // Create showtime string
    std::string showtime = newShowtimeDate + "," + newShowtimeStartTime + "," + newShowtimeEndTime;
    pendingShowtimes.push_back(showtime);
    
    // Clear input fields for next showtime
    newShowtimeDate = "";
    newShowtimeStartTime = "";
    newShowtimeEndTime = "";
    
    // Reset editing flags
    resetShowtimeEditingFlags();
}

// Function to remove a pending showtime from the list
void SFMLUIManager::removePendingShowtime(int index) {
    if (index >= 0 && index < pendingShowtimes.size()) {
        pendingShowtimes.erase(pendingShowtimes.begin() + index);
    }
}

// Function to render the list of pending showtimes
void SFMLUIManager::renderPendingShowtimes() {
    // Draw background for the pending showtimes section
    sf::RectangleShape pendingBg(sf::Vector2f(680, 30 + pendingShowtimes.size() * 30));
    pendingBg.setPosition(300, 545);
    pendingBg.setFillColor(sf::Color(40, 40, 60, 200));
    window.draw(pendingBg);
    
    // Show header for pending showtimes if any exist
    if (!pendingShowtimes.empty()) {
        sf::Text pendingHeader = createText("Pending Showtimes:", 310, 555, 16);
        pendingHeader.setFillColor(sf::Color(200, 255, 200));
        window.draw(pendingHeader);
        
        // Render each pending showtime with a remove button
        for (size_t i = 0; i < pendingShowtimes.size(); i++) {
            // Parse the showtime string
            std::istringstream ss(pendingShowtimes[i]);
            std::string date, startTime, endTime;
            
            std::getline(ss, date, ',');
            std::getline(ss, startTime, ',');
            std::getline(ss, endTime, ',');
            
            // Create formatted display text
            std::string displayText = date + " " + startTime + "-" + endTime;
            
            // Draw showtime text
            sf::Text showtimeText = createText(displayText, 330, 580 + i * 30, 14);
            window.draw(showtimeText);
            
            // Draw remove button
            sf::RectangleShape removeBtn = createStyledButton(700, 575 + i * 30, 80, 25, sf::Color(150, 30, 30));
            window.draw(removeBtn);
            
            sf::Text removeText = createText("Remove", 715, 580 + i * 30, 12);
            window.draw(removeText);
        }
    } else {
        // Show message if no showtimes added yet
        sf::Text noShowtimesText = createText("No showtimes added yet. Add one using the form above.", 310, 555, 14);
        noShowtimesText.setFillColor(sf::Color(150, 150, 150));
        window.draw(noShowtimesText);
    }
}

// Function to handle clicks on the pending showtimes UI elements
bool SFMLUIManager::handlePendingShowtimesClick(sf::Vector2i mousePos) {
    // Only process if we're in the edit movie screen and creating a new movie
    if (currentState != UIState::EDIT_MOVIE || editingMovieId != -1) {
        return false;
    }
    
    // Check for click on Add Showtime button
    sf::RectangleShape addShowtimeBtn = createStyledButton(850, 505, 120, 30, sf::Color(0, 100, 200));
    if (isButtonClicked(addShowtimeBtn, mousePos)) {
        addPendingShowtime();
        return true;
    }
    
    // Check for clicks on Remove buttons for each pending showtime
    for (size_t i = 0; i < pendingShowtimes.size(); i++) {
        sf::RectangleShape removeBtn = createStyledButton(700, 575 + i * 30, 80, 25, sf::Color(150, 30, 30));
        if (isButtonClicked(removeBtn, mousePos)) {
            removePendingShowtime(i);
            return true;
        }
    }
    
    return false;
}

// Function to clear all pending showtimes
void SFMLUIManager::clearPendingShowtimes() {
    pendingShowtimes.clear();
}
