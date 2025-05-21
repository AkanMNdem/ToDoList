import React, { createContext, useState, useEffect, useContext } from 'react';

const ThemeContext = createContext();

export const THEMES = {
  LIGHT: 'light',
  DARK: 'dark',
  SYSTEM: 'system'
};

export const ThemeProvider = ({ children }) => {
  // Initialize theme from localStorage or default to system
  const [theme, setTheme] = useState(() => {
    const savedTheme = localStorage.getItem('todo-theme');
    return savedTheme || THEMES.SYSTEM;
  });

  // Set actual theme based on preference or system setting
  const [actualTheme, setActualTheme] = useState(THEMES.LIGHT);

  useEffect(() => {
    // Save theme preference to localStorage
    localStorage.setItem('todo-theme', theme);

    // Determine actual theme to apply
    if (theme === THEMES.SYSTEM) {
      const prefersDarkMode = window.matchMedia('(prefers-color-scheme: dark)').matches;
      setActualTheme(prefersDarkMode ? THEMES.DARK : THEMES.LIGHT);
      
      // Listen for system theme changes
      const mediaQuery = window.matchMedia('(prefers-color-scheme: dark)');
      const handleChange = (e) => {
        setActualTheme(e.matches ? THEMES.DARK : THEMES.LIGHT);
      };
      
      mediaQuery.addEventListener('change', handleChange);
      return () => mediaQuery.removeEventListener('change', handleChange);
    } else {
      setActualTheme(theme);
    }
  }, [theme]);

  // Apply theme to document
  useEffect(() => {
    document.documentElement.setAttribute('data-theme', actualTheme);
  }, [actualTheme]);

  return (
    <ThemeContext.Provider value={{ theme, setTheme, actualTheme, THEMES }}>
      {children}
    </ThemeContext.Provider>
  );
};

export const useTheme = () => {
  const context = useContext(ThemeContext);
  if (context === undefined) {
    throw new Error('useTheme must be used within a ThemeProvider');
  }
  return context;
};

export default ThemeContext; 